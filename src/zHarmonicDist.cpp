#include "zHarmonicDist.h"

zHarmonicDist::zHarmonicDist()
{}

void zHarmonicDist::paint(NVGcontext *vg)
{
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, width(), height());
    nvgClosePath(vg);
    nvgFillColor(vg, nvgRGBA(0x0c,0x0c,0x0c,255));
    nvgFill(vg);
    drawGrid(vg, 10, 5, 0,0,width(),height());
    float wave[16] = {0,0,0.1,0.2,0.3,0.5,0.8,0.9,0.9,0.8,0.5,0.3,0.2,0.1,0.0, 0.0};
    //drawWave(vg, wave, 16, 0, 0, width(), height());
    drawSin(vg,  0, 0, width(), height());
}
