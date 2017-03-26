// 3DEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <mmsystem.h>
#include <d2d1.h>

#include <vector>

#include "3DEngine.h"
#include "MainWindow.h"
#include "Mesh.h"
#include "Camera.h"
#include "Device.h"

int WINAPI wWinMain (HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    std::vector<Mesh> Meshes;

    // Add meshes
    Mesh mesh = Mesh ("Cube", 8);

    std::vector<Vector3> Vertices (8);
    Vertices [0] = Vector3 (-1, 1, 1);
    Vertices [1] = Vector3 (1, 1, 1);
    Vertices [2] = Vector3 (-1, -1, 1);
    Vertices [3] = Vector3 (-1, -1, -1);
    Vertices [4] = Vector3 (-1, 1, -1);
    Vertices [5] = Vector3 (1, 1, -1);
    Vertices [6] = Vector3 (1, -1, 1);
    Vertices [7] = Vector3 (1, -1, -1);

    mesh.Vertices = Vertices;

    Meshes.push_back (mesh);

    Camera mainCamera = Camera ();

    mainCamera.Position = Vector3 (0, 0, -50);
    mainCamera.Target = Vector3 (0, 0, 0);

    Device renderDevice = Device (640, 480);

    MainWindow win;

    if (!win.Create (L"Learn to Program Windows", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    ShowWindow (win.Window (), nCmdShow);
    UpdateWindow (win.Window ());

    renderDevice.Clear(0.0, 0.0, 0.0, 1.0);

    // Run the message loop.
    MSG msg;                  // Next message from top of queue
    LONGLONG cur_time;        // Current system time
    DWORD time_count = 40;    // ms per frame, used as default if performance counter is not available
    LONGLONG perf_cnt;        // Performance timer frequency
    BOOL perf_flag = FALSE;   // Flag whether performance counter available, if false use timeGetTime()
    LONGLONG next_time = 0;   // Time to render next frame
    BOOL move_flag = TRUE;    // Flag if moved step has occurred yet

    // Is there a performance counter available?
    if (QueryPerformanceFrequency ((LARGE_INTEGER *)&perf_cnt)) {
        // Yes, set time_count and timer choice flag
        perf_flag = TRUE;
        time_count = perf_cnt / 25;    // Calculate time per frame based on frequency (25 fps, 40 milliseconds per frame)
        QueryPerformanceCounter ((LARGE_INTEGER *)&next_time);
    } else {
        // No performance counter, read in using timeGetTime
        next_time = timeGetTime ();
    }
    // Prime the message structure
    PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE);

    // Run till completed
    while (msg.message != WM_QUIT) {
        // Is there a message to process?
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
            // Dispatch the message
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        } else {

            // Do we need to move?
            if (move_flag) {
                move_flag = false;
                // Move
            }
            // Use the appropriate method to get time
            if (perf_flag) {
                QueryPerformanceCounter ((LARGE_INTEGER *)&cur_time);
            } else {
                cur_time = timeGetTime ();
            }

            // Is it time to render the frame?
            if (cur_time > next_time) {
                // Render scene
                renderDevice.Render (mainCamera, Meshes);
                // Update main window with rendered buffer
                BackBuffer buffer = renderDevice.GetBuffer ();
                win.UpdateBuffer (buffer);

                // Set time for next frame
                next_time += time_count;
                // If more than a frame behind, drop the frames
                if (next_time < cur_time)
                    next_time = cur_time + time_count;
                // Flag that we need to move objects again
                move_flag = TRUE;
            }
        }
    }

    return 0;
}