#include "zButton.h"

#include "../nanovg/nanovg.h"
#include "../draw.h"


#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zButton::zButton()
    :m_t(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zButton::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(paint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zButton::paint()
{
    printf("zButton::paint(%p,%f,%f,%f,%f)\n", this, x(),y(),width(),height());
    //glScissor(x(), y(), width(), height());
    //glEnable(GL_SCISSOR_TEST);
    //glClearColor(0, 0, 0.0, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //glDisable(GL_SCISSOR_TEST);
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(x(), y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    drawButton(vg, "Button", 0, 0, width(), height());
    //drawHZSlider(vg, x(), y(), width(), height());
    nvgEndFrame(vg);
}
