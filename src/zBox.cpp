#include "zBox.h"

zBox::zBox()
    :m_t(0)
{}

void zBox::paint(NVGcontext *vg)
{
    drawToggleBox(vg, getLabel().c_str(), 0, 0, width(), height());
}
