#pragma once
#include <QtQuick/QQuickItem>
#include "../draw.h"

struct NVGcontext;

class zWidget:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal zscale MEMBER m_zscale);
    Q_PROPERTY(qreal aspect MEMBER m_zaspect);
public:
    zWidget();
    virtual ~zWidget(){};

public slots:
    virtual void abstractPaint();
    virtual void paint(NVGcontext *vg)=0;
protected:
    qreal m_zscale, m_zaspect;
};
