#pragma once
#include "Quaternion.h"

class Slerp
{
private:
    Quaternion start;
    Quaternion end;
    
    double duration;
    double progress;

    double halfTheta;
    double cosHalfTheta;
    double sinHalfTheta;

public:
    Slerp(Quaternion start, Quaternion end, double duration);

    bool Update(double elapsed_time);
    Quaternion Current();
};
