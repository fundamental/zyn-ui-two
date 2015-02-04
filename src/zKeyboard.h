#pragma once
#include "zWidget.h"

class zKeyboard:public zWidget
{
    Q_OBJECT
public:
    zKeyboard();
    void paint(NVGcontext *);
};
