#include "zWidget.h"

#include "../nanovg/nanovg.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zWidget::zWidget()
    :m_zscale(1.0), m_zaspect(1.0)
{
}

void zWidget::abstractPaint()
{
    auto pos = mapToItem(window()->contentItem(), QPointF(0,0));
    float yy = (window()->contentItem()->height()-height())-pos.y();
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(pos.x(), yy, width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    paint(vg);
    nvgEndFrame(vg);
}
