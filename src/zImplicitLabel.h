#pragma once
#include "zWidget.h"

class zImplicitLabel:public zWidget
{
    Q_OBJECT
public:
    zImplicitLabel(QQuickItem *parent);
    void paint(NVGcontext *vg);
    friend class zModule;
};
