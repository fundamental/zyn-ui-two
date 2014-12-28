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
    printf("zKnob::paint(%p,%f,%f,%f,%f)\n", this, x(),y(),width(),height());
    glScissor(x(), y(), width(), height());
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(x(), y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    drawAltDial(vg, m_t, 0, 0, width(),height());//400, 0, 100, 100);
    nvgEndFrame(vg);
}
