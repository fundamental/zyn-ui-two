#include "zKnob.h"
#include <QtQuick/qquickwindow.h>

zKnob::zKnob()
    :m_t(0)
{}

void zKnob::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit xChanged();
    if (window())
        window()->update();
}

void zKnob::paint(NVGcontext *vg)
{
    drawAltDial(vg, m_t, 0, 0, width(),height());//400, 0, 100, 100);
}
