#include "zPanKnob.h"

void zPanKnob::paint(NVGcontext *vg)
{
    drawPanDial(vg, "PAN", m_t, 0, 0, width(),height());//400, 0, 100, 100);
}
