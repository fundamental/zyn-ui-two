#include "zWidget.h"

#include "../nanovg/nanovg.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zWidget::zWidget()
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zWidget::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(abstractPaint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
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
