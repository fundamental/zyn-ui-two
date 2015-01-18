#pragma once
#include "zWidget.h"

class zTextField:public zWidget
{
    Q_OBJECT

public:
    zTextField();
    void paint(NVGcontext *vg);
};
