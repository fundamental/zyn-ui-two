#include "zButton.h"

zButton::zButton()
    :m_t(0)
{}

void zButton::paint(NVGcontext *vg)
{
    drawButton(vg, getLabel().c_str(), 0, 0, width(), height());
    m_zaspect = textAspect(vg, getLabel().c_str());
}

void zButton::setAutoAspect(bool)
{
}
