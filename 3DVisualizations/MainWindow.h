#pragma once

#include <d2d1.h>
#include <vector>

#include "BackBuffer.h"
#include "Visualization.h"

template <class T>
void COMSafeRelease(T** ppT) {
    if (*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class MainWindow {
private:
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1Bitmap* bitmap;

    Device renderDevice;
    Visualization visualization;

    HRESULT EnsureGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint(UINT message, WPARAM wParam, LPARAM lParam);
    void    OnResize();
    void    OnKeyDown(UINT message, WPARAM wParam, LPARAM lParam);
    void    Display();

public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow();
    ~MainWindow();

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        int width = 650,
        int height = 500
    );

    HWND Window() const;
    PCWSTR ClassName() const;

    void Begin(int fps);

protected:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
};
