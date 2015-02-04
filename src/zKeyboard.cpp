#include "zKeyboard.h"
#include "../draw.h"

zKeyboard::zKeyboard()
{}

void zKeyboard::paint(NVGcontext *vg)
{
    drawKeyboard(vg, 0, 0, width(), height());
}

