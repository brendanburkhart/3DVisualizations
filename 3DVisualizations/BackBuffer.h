#pragma once

class BackBuffer {
public:
    char* buffer;

    int width, height, scanLineSize;

    BackBuffer();

    BackBuffer(int width, int height);

    void Release();

    char& operator [] (int i);
};
