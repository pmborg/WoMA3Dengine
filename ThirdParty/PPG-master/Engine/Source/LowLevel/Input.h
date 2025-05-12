#pragma once
#include "stdafx.h"

struct Input
{
    unsigned char key;
    unsigned char mouse;
    float deltaMouseX, deltaMouseY;
};

namespace Key
{
    const unsigned char NONE  = 0b0000'0000;
    const unsigned char FRONT = 0b0000'0001;
    const unsigned char BACK  = 0b0000'0010;
    const unsigned char LEFT  = 0b0000'0100;
    const unsigned char RIGHT = 0b0000'1000;
    const unsigned char UP = 0b0001'0000;
    const unsigned char DOWN = 0b0010'0000;
};

namespace Mouse
{
    const unsigned char NONE = 0b0000'0000;
    const unsigned char RMB_DOWN = 0b0000'0001;
}
