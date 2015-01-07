#include "zHSlider.h"
#include <cassert>

void zHSlider::paint(NVGcontext *vg)
{
    assert(-1 <= m_t  && m_t <= 1);
    drawHZSlider(vg, m_t, 0, 0, width(), height());
}
