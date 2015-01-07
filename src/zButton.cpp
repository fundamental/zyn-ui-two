#include "zButton.h"

zButton::zButton()
    :m_t(0)
{}

void zButton::paint(NVGcontext *vg)
{
    const char *label = "Button";
    if(!m_label.isEmpty())
        label = m_label.toLatin1().data();
    drawButton(vg, label, 0, 0, width(), height());
}
