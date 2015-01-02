#pragma once
#include "zWidget.h"

class zButton:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool t MEMBER m_t)

public:
    zButton();
    void paint(NVGcontext *vg);
private:
    bool m_t;
};
