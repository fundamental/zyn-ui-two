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
    void paint(NVGcontext *vg) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void updatePos(float delta);

signals:
    void tChanged();

private:
    qreal m_t;
    QPoint prev;
};
