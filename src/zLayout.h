#pragma once
#include "zWidget.h"
#include "zImplicitLabel.h"
#include "../layout.h"
#include <QMap>
#include <cassert>

class zLayout:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool vertical MEMBER m_vertical);
public:
    zLayout()
        :m_vertical(false)
    {}
public slots:
    void damageLayout() {doLayout();}
    void doLayout();
    virtual void paint(NVGcontext *vg){};
protected:
    virtual float layoutY() const {return 0.0f;};
    virtual float layoutH() const {return height();};
    int getLayoutChildren();
    QList<QQuickItem*> implicitChildren;
    QMap<QQuickItem *, class zImplicitLabel *> labelBoxes;
    bool m_vertical;
};
