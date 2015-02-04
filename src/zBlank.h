#pragma once
#include "zWidget.h"

class zBlank:public zWidget
{
    Q_OBJECT
public:
    zBlank();
    void paint(NVGcontext *){};
};
