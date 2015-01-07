#include "zBox.h"

zBox::zBox()
    :m_t(0)
{}

void zBox::paint(NVGcontext *vg)
{
    const char *label = "Button";
    if(!m_label.isEmpty())
        label = m_label.toLatin1().data();
    drawToggleBox(vg, label, 0, 0, width(), height());
}
