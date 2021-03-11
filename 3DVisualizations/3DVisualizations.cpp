#include "stdafx.h"
#include "MainWindow.h"

#pragma comment(lib, "winmm.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    MainWindow win;

    if (!win.Create(L"3DVisualizations", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);
    UpdateWindow(win.Window());

    win.Begin(40);

    return 0;
}
