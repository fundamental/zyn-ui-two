#pragma once
#include "zWidget.h"

class zEnvView:public zWidget
{
    Q_OBJECT
public:
    zEnvView();
    void paint(NVGcontext *vg);
private:
    float m_data[16];
};
