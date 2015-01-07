#pragma once
#include "zValuator.h"

class zVBar:public zValuator
{
public:
    void paint(NVGcontext *vg) override;
};
