#pragma once
#include "zWidget.h"

class zPowButton:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool t MEMBER m_t)

public:
    zPowButton();
    void paint(NVGcontext *vg);
private:
    bool m_t;
};
