#include "zButton.h"

zButton::zButton()
    :m_t(0)
{}

void zButton::paint(NVGcontext *vg)
{
    drawButton(vg, "Button", 0, 0, width(), height());
}
