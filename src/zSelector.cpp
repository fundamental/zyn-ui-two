#include "zSelector.h"

zSelector::zSelector()
{}

void zSelector::paint(NVGcontext *vg)
{
    drawButtonGrid(vg, 4, 4, 0, 0, width(), height());
}
