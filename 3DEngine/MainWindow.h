#pragma once

#include "stdafx.h"
#include <d2d1.h>

#include "Device.h"
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
    ID2D1SolidColorBrush    *pBrush;
    int centerX, centerY;

    BackBuffer backBuffer;
    ID2D1Bitmap *bitmap;
    
    void    CalculateLayout ();
    HRESULT CreateGraphicsResources ();
    void    DiscardGraphicsResources ();
    void    OnPaint ();
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

    void UpdateBuffer (BackBuffer buffer);

protected:

    LRESULT HandleMessage (UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
};