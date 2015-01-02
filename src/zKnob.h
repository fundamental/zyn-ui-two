#pragma once
#include "zWidget.h"

class zKnob:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    zKnob();

    qreal t() const { return m_t; }
    void setT(qreal t);
    void paint(NVGcontext *vg);

signals:
    void tChanged();

private:
    qreal m_t;
};
