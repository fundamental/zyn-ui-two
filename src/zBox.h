#pragma once
#include "zWidget.h"

class zBox:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool t MEMBER m_t)

public:
    zBox();
    void paint(NVGcontext *vg);
private:
    bool m_t;
};
