#include "zEnvView.h"

#include "../nanovg/nanovg.h"
#include "../draw.h"


#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zEnvView::zEnvView()
{
    for(int i=0; i<16; ++i)
        m_data[i] = rand()*1.0/RAND_MAX;
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zEnvView::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(paint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zEnvView::paint()
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
    float in[10] = {0.0, 0.0, 0.5, 0.2, 0.3, 0.7,-0.9,0.8,0.0,1.0};
    drawEnvEdit(vg, in, 5, 1, 0, 0, width(), height());
    nvgEndFrame(vg);
}
