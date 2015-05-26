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
        :m_vertical(false), m_damaged_layout(false), m_ch(nullptr)
    {}
public slots:
    void damageLayout() {m_damaged_layout = true;}
    void doLayout();
    BBox &layoutSubProblems(LayoutProblem &prob, BBox &parent) override;
    void layoutSolved(LayoutProblem &prob) override;
    virtual void paint(NVGcontext *vg){
        if(m_damaged_layout)
            doLayout();
        m_damaged_layout = false;
    };
protected:
    virtual float layoutY() const {return 0.0f;};
    virtual float layoutH() const {return height();};
    int getLayoutChildren();
    QList<QQuickItem*> implicitChildren;
    QMap<QQuickItem *, class zImplicitLabel *> labelBoxes;
    bool m_vertical;
    bool m_damaged_layout;
    BBox **m_ch;
};
