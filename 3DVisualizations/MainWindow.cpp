#include "stdafx.h"
#include "MainWindow.h"

#include <timeapi.h>

#pragma comment(lib, "d2d1")

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = NULL;

    // Store pointer to store in
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (MainWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    }
    else {
        pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

MainWindow::MainWindow() : m_hwnd(nullptr), pFactory(nullptr), pRenderTarget(nullptr), bitmap(nullptr) {};

MainWindow::~MainWindow() {
    if (bitmap) {
        bitmap->Release();
        bitmap = nullptr;
    }
}

BOOL MainWindow::Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    int width,
    int height
) {
    WNDCLASS wc = { };

    wc.lpfnWndProc = MainWindow::WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = ClassName();
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        0, ClassName(), lpWindowName, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, 0, 0, GetModuleHandle(NULL), this
    );

    return (m_hwnd ? TRUE : FALSE);
}

HWND MainWindow::Window() const { return m_hwnd; }

PCWSTR MainWindow::ClassName() const { return L"3DVisualizationsMain"; }

HRESULT MainWindow::EnsureGraphicsResources() {
    HRESULT hr = S_OK;

    if (pRenderTarget == NULL) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        renderDevice = Device(size.width, size.height);
    }

    return hr;
}

void MainWindow::DiscardGraphicsResources() {
    COMSafeRelease(&pRenderTarget);
}

void MainWindow::OnPaint(UINT message, WPARAM wParam, LPARAM lParam) {
    HRESULT hr = EnsureGraphicsResources();

    if (bitmap == nullptr) {
        DefWindowProc(m_hwnd, message, wParam, lParam);
        return;
    }

    if (!SUCCEEDED(hr)) {
        return;
    }

    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);

    pRenderTarget->BeginDraw();

    D2D1_SIZE_F size = pRenderTarget->GetSize();
    D2D1_SIZE_F bitmapSize = bitmap->GetSize();

    pRenderTarget->DrawBitmap(
        bitmap,
        D2D1::RectF(
            0,
            0,
            size.width,
            size.height),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF(
            0,
            0,
            bitmapSize.width,
            bitmapSize.height)
    );

    hr = pRenderTarget->EndDraw();

    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
        DiscardGraphicsResources();
    }

    EndPaint(m_hwnd, &ps);
}

void MainWindow::OnResize() {
    if (pRenderTarget == nullptr) {
        return;
    }

    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    pRenderTarget->Resize(size);
    renderDevice = Device(size.width, size.height);

    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnKeyDown(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (wParam) {
    case VK_ESCAPE:
        PostMessage(m_hwnd, WM_CLOSE, NULL, NULL);
    }

    DefWindowProc(m_hwnd, message, wParam, lParam);
}

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
    {
        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
            return -1;
        }
        return 0;
    }
    case WM_PAINT:
    {
        OnPaint(message, wParam, lParam);
        return 0;
    }
    case WM_SIZE:
    {
        OnResize();
        return 0;
    }
    case WM_KEYDOWN:
    {
        OnKeyDown(message, wParam, lParam);
        return 0;
    }
    case WM_DESTROY:
    {
        DiscardGraphicsResources();
        COMSafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(m_hwnd, message, wParam, lParam);
    }

    return TRUE;
}

void MainWindow::Begin(int fps) {
    // Run the message loop.
    MSG msg;                    // Next message from top of queue
    UINT32 frame_duration = 1000/fps; // ms per frame, used as default if performance counter is not available
    LONGLONG perf_cnt;          // Performance timer frequency if available
    BOOL perf_flag = FALSE;     // Flag whether performance counter available, if false use timeGetTime()
    LONGLONG cur_time;          // Current system time
    LONGLONG next_time = 0;     // Time to render next frame
    LONGLONG last_time = 0;     // System time at last update

    // Is there a performance counter available?
    if (QueryPerformanceFrequency((LARGE_INTEGER*)&perf_cnt)) {
        perf_flag = TRUE;
        frame_duration = (UINT32)perf_cnt / fps;
        QueryPerformanceCounter((LARGE_INTEGER*)&cur_time);
    }
    else {
        cur_time = timeGetTime();
        perf_cnt = 1000;
    }

    last_time = cur_time;
    next_time = cur_time;
    droppedFrames = 0;

    // Initialize the message structure
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

    while (msg.message != WM_QUIT) {
        // Is there a message to process?
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // Dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // Use the appropriate method to get time
            if (perf_flag) {
                QueryPerformanceCounter((LARGE_INTEGER*)&cur_time);
            }
            else {
                cur_time = timeGetTime();
            }

            if (cur_time < next_time) {
                continue;
            }

            if (!SUCCEEDED(EnsureGraphicsResources())) {
                continue;
            }

            double elapsed_seconds = (cur_time - last_time) / static_cast<double>(perf_cnt);
            visualization.Update(elapsed_seconds);
            visualization.Render(renderDevice);
            Display();
            
            // Set time for next frame
            next_time += frame_duration;
            last_time = cur_time;
            // If more than a frame behind, drop the frames
            if (next_time < cur_time) {
                next_time = cur_time + frame_duration;
                droppedFrames++;
            }

            if (droppedFrames > 3) {
                OutputDebugString(L"Dropped four frames\n");
                droppedFrames = 0;
            }
        }
    }
}

void MainWindow::Display() {
    if (bitmap) {
        bitmap->Release();
        bitmap = nullptr;
    }

    auto buffer = renderDevice.GetBuffer();

    // Create bitmap
    HRESULT result = pRenderTarget->CreateBitmap(
        D2D1::SizeU(buffer.width, buffer.height),
        buffer.data.data(),
        (UINT32)buffer.scanLineSize,
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &bitmap);

    // Update screen
    OnPaint(WM_PAINT, NULL, NULL);
}
