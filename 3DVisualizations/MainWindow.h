#pragma once

#include <d2d1.h>
#include <vector>

#include "Device.h"
#include "Camera.h"
#include "BackBuffer.h"

template <class T>
void SafeRelease (T **ppT) {
    if (*ppT) {
        (*ppT)->Release ();
        *ppT = NULL;
    }
}

class MainWindow {

    ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget;
    
    Device renderDevice;
    Camera *renderCamera;
    std::vector<Mesh> *meshes;

    ID2D1Bitmap *bitmap;
    
    HRESULT CreateGraphicsResources ();
    void    DiscardGraphicsResources ();
    void    OnPaint (UINT message, WPARAM wParam, LPARAM lParam);
    void    Resize ();
    void    Key (UINT message, WPARAM wParam, LPARAM lParam);

public:
    static LRESULT CALLBACK WindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow ();

    ~MainWindow ();

    BOOL Create (
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = 650,
        int nHeight = 500,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    );

    HWND Window () const;

    PCWSTR ClassName () const;

    void Render ();

    void setCamera (Camera* camera);
    void setMeshList (std::vector<Mesh>* meshes);

protected:

    LRESULT HandleMessage (UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
};