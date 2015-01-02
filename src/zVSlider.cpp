#include "zVSlider.h"

void zVSlider::paint(NVGcontext *vg)
{
    drawVZSlider(vg, m_t, 0, 0, width(), height());
}
