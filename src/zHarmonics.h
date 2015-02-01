#pragma once
#include "zWidget.h"

class zHarmonics:public zWidget
{
    Q_OBJECT
public:
    zHarmonics();
    void paint(NVGcontext *vg);

    //-1 or index of grabbable point
    Q_INVOKABLE int    objNear(QPointF);
    Q_INVOKABLE void   objMove(int, QPointF);
    Q_INVOKABLE void   objSet(int, float);
private:
    float m_data[32];
};
