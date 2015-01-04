#pragma once
#include "zWidget.h"

class zBlank:public zWidget
{
    Q_OBJECT
public:
    zBlank();
    void abstractPaint();
    void paint(NVGcontext *){};
public slots:
    void handleWindowChanged(QQuickWindow *win);
};
