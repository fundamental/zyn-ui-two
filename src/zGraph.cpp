#include <QtQuick/qquickwindow.h>
#include "../nanovg/nanovg.h"
#include "../draw.h"
NVGcontext *initVG();

#include "zGraph.h"
zGraph::zGraph()
     :m_value(0.1)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zGraph::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(paint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zGraph::paint()
{
    printf("zGraph::paint(%p,%f,%f,%f,%f)\n", this,
            x(),y(),width(),height());
    //glScissor(x(), widget->window()->height()-height()-y(), width(), height());
    //glEnable(GL_SCISSOR_TEST);
    //glClearColor(0, 0, 0.0, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //glDisable(GL_SCISSOR_TEST);
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(x(), y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, width(), height());
    nvgClosePath(vg);
    nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    nvgFill(vg);
    drawGrid(vg, 10, 10, 0,0,width(),height());
    drawSin(vg,  0, 0, width(), height());
    nvgEndFrame(vg);
}
