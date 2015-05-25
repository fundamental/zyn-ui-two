#include "zStatusPane.h"

zStatusPane::zStatusPane()
{}

void zStatusPane::paint(NVGcontext *vg)
{
    const float padh = 0.1;
    const float padw = 0.01;

    //draw the black box
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, width(), height());
    nvgClosePath(vg);
    nvgFillColor(vg, nvgRGBA(0x0c,0x0c,0x0c,255));
    nvgFill(vg);

    drawLeftLabel(vg, "Preset", padw*width(), padh*height(),
            (1-2*padw)*width(), (0.5-padh)*height());
    drawLeftLabel(vg, "KNOB NAME", padw*width(), (0.5+padh)*height(),
            (1-2*padw)*width(), (0.5-padh)*height());
    drawRightLabel(vg, "VALUE", padw*width(), (0.5+padh)*height(),
            (1-2*padw)*width(), (0.5-padh)*height());

    nvgBeginPath(vg);
    nvgMoveTo(vg, padw*width(), 0.5*height());
    nvgLineTo(vg, (1-padw)*width(), 0.5*height());
    nvgStrokeColor(vg, nvgRGBA(0x32, 0xb7, 0xdd, 255));
    nvgStroke(vg);
}
