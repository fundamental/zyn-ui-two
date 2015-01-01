#include "zKnob.h"

#include "../nanovg/nanovg.h"
#include "../draw.h"


#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zKnob::zKnob()
    :m_t(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zKnob::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit xChanged();
    if (window())
        window()->update();
}

void zKnob::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(paint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zKnob::paint()
{
    auto pos = mapToItem(window()->contentItem(), QPointF(0,0));
    float yy = (window()->contentItem()->height()-height())-pos.y();
    printf("zKnob::paint(%p,%f,%f,%f,%f)\n", this, pos.x(),yy,width(),height());
    //printf("zKnob(%f,%f,%f)\n", height(), window()->contentItem()->height(), pos.y());
    //glScissor(x(), y(), width(), height());
    //glEnable(GL_SCISSOR_TEST);
    //glClearColor(0, 0, 0.0, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //glDisable(GL_SCISSOR_TEST);
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(pos.x(), yy, width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    drawAltDial(vg, m_t, 0, 0, width(),height());//400, 0, 100, 100);
    nvgEndFrame(vg);
}
