#include "zGraph.h"

zGraph::zGraph()
{}

void zGraph::paint(NVGcontext *vg)
{
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, width(), height());
    nvgClosePath(vg);
    nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    nvgFill(vg);
    drawGrid(vg, 10, 10, 0,0,width(),height());
    drawSin(vg,  0, 0, width(), height());
}
