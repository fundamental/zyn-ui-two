#pragma once
#include "zWidget.h"

class zSelector:public zWidget
{
    Q_OBJECT
public:
    zSelector();
    void paint(NVGcontext *vg);
};
