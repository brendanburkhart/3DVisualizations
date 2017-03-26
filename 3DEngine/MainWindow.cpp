#include "stdafx.h"
#include "MainWindow.h"

#pragma comment(lib, "d2d1")

LRESULT CALLBACK MainWindow::WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow *pThis = NULL;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (MainWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    } else {
        pThis = (MainWindow*)GetWindowLongPtr (hwnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->HandleMessage (uMsg, wParam, lParam);
    } else {
        return DefWindowProc (hwnd, uMsg, wParam, lParam);
    }
}

MainWindow::MainWindow () : m_hwnd (NULL), pFactory (NULL), pRenderTarget (NULL), bitmap(NULL) {};

MainWindow::~MainWindow () {
    if (bitmap) {
        bitmap->Release ();
        bitmap = NULL;
    }
}

BOOL MainWindow::Create (
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu
) {

    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = MainWindow::WindowProc;
    wc.hInstance = GetModuleHandle (NULL);
    wc.lpszClassName = ClassName ();

    RegisterClass (&wc);

    m_hwnd = CreateWindowEx (
        dwExStyle, ClassName (), lpWindowName, dwStyle, x, y,
        nWidth, nHeight, hWndParent, hMenu, GetModuleHandle (NULL), this
    );

    return (m_hwnd ? TRUE : FALSE);
}

HWND MainWindow::Window () const { return m_hwnd; }

PCWSTR MainWindow::ClassName () const { return L"3D Graphics Engine"; }

HRESULT MainWindow::CreateGraphicsResources () {
    HRESULT hr = S_OK;

    if (pRenderTarget == NULL) {
        RECT rc;
        GetClientRect (m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU (rc.right, rc.bottom);
        
        renderDevice = Device (size.width, size.height);
        
        hr = pFactory->CreateHwndRenderTarget (
            D2D1::RenderTargetProperties (),
            D2D1::HwndRenderTargetProperties (m_hwnd, size),
            &pRenderTarget);

    }
    return hr;
}

void MainWindow::DiscardGraphicsResources () {
    SafeRelease (&pRenderTarget);
    renderDevice.Release ();
}

void MainWindow::OnPaint (UINT message, WPARAM wParam, LPARAM lParam) {
    HRESULT hr = CreateGraphicsResources ();

    if (bitmap == nullptr) {
        OutputDebugString (L"Oh noeesssss!!\n\n");
        DefWindowProc (m_hwnd, message, wParam, lParam);
        return;
    }

    if (SUCCEEDED (hr)) {
        PAINTSTRUCT ps;
        BeginPaint (m_hwnd, &ps);

        pRenderTarget->BeginDraw ();

        D2D1_SIZE_F size = pRenderTarget->GetSize ();

        pRenderTarget->DrawBitmap (
            bitmap,
            D2D1::RectF (
                0,
                0,
                size.width,
                size.height),
            1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            D2D1::RectF (
                0,
                0,
                (FLOAT) renderDevice.getWidth(),
                (FLOAT) renderDevice.getHeight())
        );

        hr = pRenderTarget->EndDraw ();
        if (FAILED (hr) || hr == D2DERR_RECREATE_TARGET) {
            DiscardGraphicsResources ();
        }
        EndPaint (m_hwnd, &ps);
    }
}

void MainWindow::Resize () {
    if (pRenderTarget != NULL) {
        RECT rc;
        GetClientRect (m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU (rc.right, rc.bottom);

        pRenderTarget->Resize (size);

        renderDevice.Release ();
        renderDevice = Device (size.width, size.height);
        
        InvalidateRect (m_hwnd, NULL, FALSE);
    }
}

void MainWindow::Key (UINT message, WPARAM wParam, LPARAM lParam) {
    switch (wParam) {
    case VK_ESCAPE:
        PostMessage (m_hwnd, WM_CLOSE, NULL, NULL);
    }

    DefWindowProc (m_hwnd, message, wParam, lParam);
}

LRESULT MainWindow::HandleMessage (UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
    {
        if (FAILED (D2D1CreateFactory (
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
            return -1;
        }
        return 0;
    }
    case WM_PAINT:
    {
        OnPaint (message, wParam, lParam);
        return 0;
    }
    case WM_SIZE:
    {
        Resize ();
        return 0;
    }
    case WM_KEYDOWN:
    {
        Key (message, wParam, lParam);
        return 0;
    }
    case WM_DESTROY:
    {
        DiscardGraphicsResources ();
        SafeRelease (&pFactory);
        PostQuitMessage (0);
        return 0;
    }
    default:
        return DefWindowProc (m_hwnd, message, wParam, lParam);
    }
    return TRUE;
}

void MainWindow::Render () {
    renderDevice.Clear (Color4(0, 0, 0, 1.0));

    renderDevice.Render (*renderCamera, *meshes);

    BackBuffer buffer = renderDevice.GetBuffer ();

    if (bitmap) {
        bitmap->Release ();
        bitmap = nullptr;
    }

    // Create bitmap
    HRESULT result = pRenderTarget->CreateBitmap (
        D2D1::SizeU (buffer.width, buffer.height),
        buffer.buffer,
        (UINT32)buffer.scanLineSize,
        D2D1::BitmapProperties (D2D1::PixelFormat (DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &bitmap);

    // Update screen
    OnPaint (WM_PAINT, NULL, NULL);
}

void MainWindow::setCamera (Camera* camera) {
    this->renderCamera = camera;
}

void MainWindow::setMeshList (std::vector<Mesh>* meshes) {
    this->meshes = meshes;
}