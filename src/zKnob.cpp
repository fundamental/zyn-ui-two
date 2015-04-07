#include "zKnob.h"
#include <QtQuick/qquickwindow.h>

zKnob::zKnob()
    :m_t(rand()*1.0/RAND_MAX)
{}

void zKnob::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit xChanged();
    auto *root = dynamic_cast<zWidget*>(window()->contentItem()->childItems()[0]);
    if(root)
        root->tryDamage(this);
    if(window())
        window()->update();
}

void zKnob::paint(NVGcontext *vg)
{
    drawAltDial(vg, m_t, 0, 0, width(),height());
}
