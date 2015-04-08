#pragma once
#include <QtQuick/QQuickItem>
#include <QRectF>
#include "../draw.h"
#include "layout/constraint-layout.h"

struct NVGcontext;

class zWidget:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal zscale  MEMBER m_zscale);
    Q_PROPERTY(qreal aspect  MEMBER m_zaspect);
    Q_PROPERTY(bool  expandable MEMBER m_zexpandable);
    Q_PROPERTY(QString label MEMBER m_label);
public:
    zWidget(QQuickItem *parent=0);
    virtual ~zWidget(){};
    std::string getLabel() const;
    void tryDamage(QQuickItem *);

public slots:
    virtual void abstractPaint(QRectF);
    virtual void paint(NVGcontext *vg)=0;
    void handleWindowChanged(QQuickWindow *win);
    void handleSync();
    virtual BBox &layoutSubProblems(LayoutProblem &prob, BBox &parent);
    virtual void abstractDamageLayout(){damageLayout();};
    virtual void damageLayout(){};
protected:
    qreal m_zscale, m_zaspect;
    QString m_label;
    bool m_zexpandable;
    QRectF m_damage;
    BBox   bbox;
};
void setBounds(QQuickItem &o, const BBox &box);
void setBounds(QQuickItem &o, float x, float y, float w, float h);
