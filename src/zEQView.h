#pragma once
#include "zWidget.h"

class zEQView:public zWidget
{
    Q_OBJECT
public:
    zEQView();
    void paint(NVGcontext *vg);

private:
    float m_data[16];
};
