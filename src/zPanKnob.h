#pragma once
#include "zValuator.h"

class zPanKnob:public zValuator
{
    Q_OBJECT
public:
    void paint(NVGcontext *vg);
};
