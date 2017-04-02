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
#include "MeshLoader.h"

int WINAPI wWinMain (HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    // This next bit gets the path to the .exe and stores it into filePath

    #define filePathLength 150

    wchar_t fileName [filePathLength];

    int bytes = GetModuleFileName (NULL, fileName, filePathLength);
    if (bytes == 0)
        return -1;
    else if (bytes == 150)
        return -5;

    // Get the directory the program is executing in
    std::wstring directory;
    std::wstring fileNameString (fileName);

    const size_t last_slash_idx = fileNameString.rfind ('\\');
    if (std::string::npos != last_slash_idx) {
        directory = fileNameString.substr (0, last_slash_idx);
    }
    
    std::vector<Mesh> Meshes;

    //Mesh cube = Mesh ("Cube", 8, 12);
    //std::vector<Vector3> Vertices (8);
    //Vertices [0] = Vector3 (-3, 3, 3);
    //Vertices [1] = Vector3 (3, 3, 3);
    //Vertices [2] = Vector3 (-3, -3, 3);
    //Vertices [3] = Vector3 (3, -3, 3);
    //Vertices [4] = Vector3 (-3, 3, -3);
    //Vertices [5] = Vector3 (3, 3, -3);
    //Vertices [6] = Vector3 (-3, -3, -3);
    //Vertices [7] = Vector3 (3, -3, -3);
    //std::vector<Mesh::Face> Faces (12);
    //Faces [0] = Mesh::Face ( 0, 1, 2);
    //Faces [1] = Mesh::Face ( 1, 2, 3);
    //Faces [2] = Mesh::Face ( 1, 3, 7);
    //Faces [3] = Mesh::Face ( 1, 5, 7);
    //Faces [4] = Mesh::Face ( 0, 1, 4);
    //Faces [5] = Mesh::Face ( 1, 4, 5);
    //Faces [6] = Mesh::Face ( 2, 3, 7);
    //Faces [7] = Mesh::Face ( 2, 6, 7);
    //Faces [8] = Mesh::Face ( 2, 4, 6);
    //Faces [9] = Mesh::Face ( 0, 4, 2);
    //Faces [10] = Mesh::Face ( 4, 5, 7);
    //Faces [11] = Mesh::Face ( 4, 6, 7);

    std::wstring resourceFile = directory + L"\\monkey.json";

    MeshLoader::LoadMesh (Meshes, resourceFile);

    Camera mainCamera = Camera ();

    mainCamera.Position = Vector3 (0, 0, 10);
    mainCamera.Target = Vector3::Origin ();

    MainWindow win;

    if (!win.Create (L"Learn to Program Windows", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    win.setCamera (&mainCamera);
    win.setMeshList (&Meshes);

    ShowWindow (win.Window (), nCmdShow);
    UpdateWindow (win.Window ());

    // Run the message loop.
    MSG msg;                  // Next message from top of queue
    LONGLONG cur_time;        // Current system time
    UINT32 time_count = 100;    // ms per frame, used as default if performance counter is not available
    LONGLONG perf_cnt;        // Performance timer frequency
    BOOL perf_flag = FALSE;   // Flag whether performance counter available, if false use timeGetTime()
    LONGLONG next_time = 0;   // Time to render next frame
    BOOL move_flag = TRUE;    // Flag if moved step has occurred yet

    // Is there a performance counter available?
    if (QueryPerformanceFrequency ((LARGE_INTEGER *)&perf_cnt)) {
        // Yes, set time_count and timer choice flag
        perf_flag = TRUE;
        time_count = (UINT32)perf_cnt / 10;    // Calculate time per frame based on frequency (25 fps, 40 milliseconds per frame)
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
                move_flag = FALSE;
                for (auto &mesh : Meshes) {
                    mesh.Rotation = Vector3 (mesh.Rotation.X + 0.05f, mesh.Rotation.Y + 0.05f, mesh.Rotation.Z);
                }
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
                win.Render ();
                // Set time for next frame
                next_time += time_count;
                // If more than a frame behind, drop the frames
                if (next_time < cur_time) {
                    next_time = cur_time + time_count;
                    OutputDebugString (L"Dropped a frame :(\n");
                }
                // Flag that we need to move objects again
                move_flag = TRUE;
            }
        }
    }

    return 0;
}