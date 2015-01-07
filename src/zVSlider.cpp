#include "zVSlider.h"
#include <cassert>

void zVSlider::paint(NVGcontext *vg)
{
    assert(-1 <= m_t  && m_t <= 1);
    drawVZSlider(vg, m_t, 0, 0, width(), height());
}
