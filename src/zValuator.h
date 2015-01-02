#pragma once
#include "zWidget.h"

class zValuator:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal t MEMBER m_t NOTIFY tChanged);

public:
    zValuator();
    virtual ~zValuator(){};

signals:
    void tChanged();

public slots:
    void update();
    virtual void paint(NVGcontext *vg)=0;

protected:
    qreal m_t;
};
