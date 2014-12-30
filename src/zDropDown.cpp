#include <QtQuick/qquickwindow.h>
#include "../nanovg/nanovg.h"
#include "../draw.h"
NVGcontext *initVG();

#include "zDropDown.h"
zDropDown::zDropDown()
     :m_value(0.1)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zDropDown::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(paint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zDropDown::paint()
{
    printf("zDropDown::paint(%p,%f,%f,%f,%f)\n", this,
            x(),y(),width(),height());
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(x(), y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    drawOptButton(vg, "Choice", 0, 0, width(), height());
    nvgEndFrame(vg);
}
