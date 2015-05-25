#include "zDropDown.h"
#include <cassert>

zDropDown::zDropDown()
     :m_value(1)
{
}

void zDropDown::paint(NVGcontext *vg)
{
    //Fix for possible compiler bug
    assert(m_zexpandable == false);
    drawOptButton(vg, "Choice", 0, 0, width(), height());
    m_zaspect = textAspect(vg, getLabel().c_str());
}
