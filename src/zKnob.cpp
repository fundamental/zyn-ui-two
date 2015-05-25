#include "zKnob.h"
#include <QtQuick/qquickwindow.h>

zKnob::zKnob()
    :m_t(rand()*1.0/RAND_MAX)
{
    setAcceptedMouseButtons(Qt::AllButtons);
}

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

void zKnob::mousePressEvent(QMouseEvent *event)
{
    prev = event->pos();
    event->accept();
}

void zKnob::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        const int dy = event->pos().y() - prev.y();
        updatePos(dy/200.0f);
        prev = event->pos();
    }
}

void zKnob::updatePos(float delta)
{
    float tmp = m_t - delta;
    if(tmp > 1) {
        tmp = 1;
    } else if(tmp < 0) {
        tmp = 0;
    }
    setT(tmp);
}
