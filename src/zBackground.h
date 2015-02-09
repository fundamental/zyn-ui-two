#pragma once
#include "zWidget.h"
#include "../draw.h"

class zBackground:public zWidget
{
    Q_OBJECT
public:
    zBackground() {}
    void paint(NVGcontext *vg)
    {
        nvgBeginPath(vg);
        nvgRect(vg, 0, 0, width(), height());
        nvgFillColor(vg, nvgRGBA(0x2d,0x2d,0x2d, 255));
        nvgFill(vg);
    };
};
