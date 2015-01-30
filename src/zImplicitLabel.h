#pragma once
#include "zWidget.h"

class zImplicitLabel:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool t MEMBER m_t)
public:
    zImplicitLabel();
    void paint(NVGcontext *vg);
    bool dummy() { return false;}
private:
    bool m_t;
};
