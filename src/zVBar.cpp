#include "zVBar.h"

void zVBar::paint(NVGcontext *vg)
{
    drawVSlider(vg, m_t, 0, 0, width(), height());
}
