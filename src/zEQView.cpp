#include "zEQView.h"
#include "../dsp/filter.h"
#include <QtQuick/qquickwindow.h>

zEQView::zEQView()
{
    float in[10] = {0.0, 0.0, 0.5, 0.2, 0.3, 0.7,-0.9,0.8,0.0,1.0};
    for(int i=0; i<16; ++i)
        m_data[i] = in[i];
}

void zEQView::paint(NVGcontext *vg)
{
    Response l = getLpf(800.0);
    Response b = getBpf(60.0,3.0);
    for(int i=0; i<128; ++i) {
        l.mag[i] /= 20;
        b.mag[i] /= 20;
    }
    drawEqGrid(vg, 0, 0, width(), height());
    drawEqLine(vg, l.mag, 128, 0, 0, width(), height());
    drawEqLine(vg, b.mag, 128, 0, 0, width(), height());
}
