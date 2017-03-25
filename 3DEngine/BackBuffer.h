#pragma once

class BackBuffer {
    char *buffer;
    int *memBuffer;

    int pixelWidth, pixelHeight;

public:
    BackBuffer ();

    BackBuffer (int width, int height);

    ~BackBuffer ();

    char &operator [] (int i);

    int* getBuffer ();

    int getBufferWidth ();
    int getBufferHeight ();
};