#include <QtGui/QGraphicsSceneMouseEvent>
#include "../nanovg/nanovg.h"
#include "../draw.h"

#include "zGraph.h"
zGraph::zGraph(QDeclarativeItem *parent)
{
     setFlag(QGraphicsItem::ItemHasNoContents, false);
     setKeepMouseGrab(true);
     m_value = 0.1;
     //setMouseTracking(true); 
}
NVGcontext *initVG();
//
void zGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    printf("zGraph::paint(%p,%f,%f,%f,%f)\n", this,
            x(),y(),width(),height());
    painter->beginNativePainting();
    glScissor(x(), widget->window()->height()-height()-y(), width(), height());
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    NVGcontext *vg = (NVGcontext*)initVG();
    //drawButton(vg, "banana", 0, 0, 100, 50);
    printf("view(%f vs %f)\n", widget->window()->height()-height()-y(), y());
    glViewport(x(), widget->window()->height()-height()-y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, width(), height());
    nvgClosePath(vg);
    nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    nvgFill(vg);
    //drawButton(vg, "banana", 100, 25, 100, 50);
    //drawOptButton(vg, "opt", 200, 25, 100, 50);
    //drawButtonGrid(vg, 4,4, 300, 0, 100, 100);
    //drawAltDial(vg, m_value, 0, 0, width(),height());//400, 0, 100, 100);
    //drawGrid(vg, 8, 8, 0,0,width(),height());
    drawGrid(vg, 10, 10, 0,0,width(),height());
    drawSin(vg,  0, 0, width(), height());
    nvgEndFrame(vg);
    
}

void zGraph::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    auto p = event->pos();
    printf("mouse: %f %f\n",p.x(), p.y());
}
        
bool zGraph::sceneEvent(QEvent *event)
{
    printf("zGraph EVENT...\n");
    return false;

}
