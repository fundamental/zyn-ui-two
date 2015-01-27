#include "zImplicitLabel.h"

zImplicitLabel::zImplicitLabel()
{}

void zImplicitLabel::paint(NVGcontext *vg)
{
    drawButton(vg, getLabel().c_str(), 0, 0, width(), height());
    m_zaspect = textAspect(vg, getLabel().c_str());
}
