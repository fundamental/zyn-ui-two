#include "zVAltSlider.h"

void zVAltSlider::paint(NVGcontext *vg)
{
    drawVAltSlider(vg, m_t, 0, 0, width(), height());
}
