#include "zDropDown.h"

zDropDown::zDropDown()
     :m_value(1)
{
}

void zDropDown::paint(NVGcontext *vg)
{
    drawOptButton(vg, "Choice", 0, 0, width(), height());
}
