#pragma once
#include "zWidget.h"

class zEnvView:public zWidget
{
    Q_OBJECT
public:
    zEnvView();
    void paint(NVGcontext *vg);


    //-1 or index of grabbable point
    Q_INVOKABLE int     objNear(QPointF);
    Q_INVOKABLE QPointF objMove(int, QPointF);
    Q_INVOKABLE QRectF  objBound(int);

private:
    float m_data[16];
};
