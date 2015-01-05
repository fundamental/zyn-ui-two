#include "zEnvView.h"
#include <QtQuick/qquickwindow.h>

zEnvView::zEnvView()
{
    float in[10] = {0.0, 0.0, 0.5, 0.2, 0.3, 0.7,-0.9,0.8,0.0,1.0};
    for(int i=0; i<16; ++i)
        m_data[i] = in[i];
}

void zEnvView::paint(NVGcontext *vg)
{
    drawEnvEdit(vg, m_data, 5, 1, 0, 0, width(), height());
}

int zEnvView::objNear(QPointF p)
{
    const float threshold = 0.05;
    float best = 10;
    int   bestInd = -1;
    for(int i=0; i<5; ++i) {
        QPointF d = QPointF(m_data[2*i+1],m_data[2*i+0])-p;
        const float dist = sqrtf(d.x()*d.x() + d.y()*d.y());
        if(dist < best) {
            best = dist;
            bestInd = i;
        }
    }
    if(best < threshold)
        return bestInd;
    return -1;
}

QPointF zEnvView::objMove(int x, QPointF delta)
{
    const QPointF cur(m_data[2*x+1], m_data[2*x+0]);
    const QRectF bbox = objBound(x);
    QPointF dest = cur + delta;
    if(dest.x() < bbox.left())   dest.setX(bbox.left());
    if(dest.x() > bbox.right())  dest.setX(bbox.right());
    if(dest.y() < bbox.top())    dest.setY(bbox.top());
    if(dest.y() > bbox.bottom()) dest.setY(bbox.bottom());
    m_data[2*x+1] = dest.x();
    m_data[2*x+0] = dest.y();

    if(window())
        window()->update();

    return dest;
}

QRectF zEnvView::objBound(int x)
{
    if(x == 0) {
        return QRectF(0,-1,0,2);
    } else if(x==4) {
        return QRectF(1,-1,0,2);
    } else if(0<x && x<4) {
        const float x1 = m_data[2*(x-1)+1];
        const float x2 = m_data[2*(x+1)+1];
        return QRectF(x1, -1, x2-x1, 2);
    }

    return QRectF(0,0,0,0);
}
