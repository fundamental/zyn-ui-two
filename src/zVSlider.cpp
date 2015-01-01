#include "zVSlider.h"

#include "../nanovg/nanovg.h"
#include "../draw.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

void zVSlider::paint()
{
    printf("zVSlider::paint(%p,%f,%f,%f,%f)\n", this, x(),y(),width(),height());
    //glScissor(x(), y(), width(), height());
    //glEnable(GL_SCISSOR_TEST);
    //glClearColor(0, 0, 0.0, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //glDisable(GL_SCISSOR_TEST);
    auto pos = mapToItem(window()->contentItem(), QPointF(0,0));
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(pos.x(), pos.y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    //drawAltDial(vg, m_t, 0, 0, width(),height());//400, 0, 100, 100);
    drawVZSlider(vg, m_t, 0, 0, width(), height());
    nvgEndFrame(vg);
}
