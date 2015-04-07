#include "zKeyboard.h"
#include "../draw.h"

zKeyboard::zKeyboard()
{}

void zKeyboard::paint(NVGcontext *vg)
{
    nvgBeginPath(vg);
    nvgRect(vg, 0, 0, width(), height());
    nvgFillColor(vg, nvgRGBA(0x0,0x0,0x0, 255));
    nvgFill(vg);
    float inset = 0.06*height();
    drawKeyboard(vg, inset, inset, width()-2*inset, height()-2*inset);
}

