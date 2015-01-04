#pragma once
#include "zWidget.h"

class zBlank:public zWidget
{
    Q_OBJECT
public:
    void abstractPaint();
    void paint(NVGcontext *){};
};
