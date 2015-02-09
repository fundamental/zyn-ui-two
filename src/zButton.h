#pragma once
#include "zWidget.h"

class zButton:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool t MEMBER m_t)
    Q_PROPERTY(bool auto_aspect READ dummy WRITE setAutoAspect)
    Q_PROPERTY(QString renderer MEMBER m_renderer)

public:
    zButton();
    void paint(NVGcontext *vg);
    void setAutoAspect(bool);
    bool dummy() { return false;}
private:
    bool m_t;
    QString m_renderer;
};
