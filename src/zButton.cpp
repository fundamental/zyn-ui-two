#include "zButton.h"

zButton::zButton()
    :m_t(0), m_renderer("default")
{}

void zButton::paint(NVGcontext *vg)
{
    if(m_renderer == "default")
        drawButton(vg, getLabel().c_str(), 0, 0, width(), height());
    else
        drawRelaxedButton(vg, getLabel().c_str(), 0, 0, width(), height());
    m_zaspect = textAspect(vg, getLabel().c_str());
}

void zButton::setAutoAspect(bool)
{
}
