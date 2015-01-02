#pragma once
#include <QtQuick/QQuickItem>
#include "../draw.h"

struct NVGcontext;

class zWidget:public QQuickItem
{
    Q_OBJECT
public:
    zWidget();
    virtual ~zWidget(){};

public slots:
    void abstractPaint();
    virtual void paint(NVGcontext *vg)=0;
    void handleWindowChanged(QQuickWindow *win);
};
