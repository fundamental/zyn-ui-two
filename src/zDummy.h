#pragma once
#include "zWidget.h"

class zDummy:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal t MEMBER m_t);
public:
    zDummy(){};
    void paint(NVGcontext *vg){};
private:
    qreal m_t;
};
