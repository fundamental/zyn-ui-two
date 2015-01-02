#include "zHSlider.h"

void zHSlider::paint(NVGcontext *vg)
{
    drawHZSlider(vg, m_t, 0, 0, width(), height());
}
