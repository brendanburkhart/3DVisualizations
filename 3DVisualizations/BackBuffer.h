#pragma once

#include <vector>

class BackBuffer {
public:
    std::vector<char> data;
    int width, height, scanLineSize;

    BackBuffer();
    BackBuffer(int width, int height);

    char& operator [] (int index);
};
