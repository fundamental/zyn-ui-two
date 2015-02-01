#include "zHarmonics.h"
#include <QtQuick/qquickwindow.h>

zHarmonics::zHarmonics()
{
    for(int i=0; i<32; ++i)
        m_data[i] = 0.0;
}

void zHarmonics::paint(NVGcontext *vg)
{
    nvgBeginPath(vg);
    nvgRect(vg,0,0,width(),height());
    nvgFillColor(vg, nvgRGBA(0x06,0x22,0x37, 255));
    nvgFill(vg);

    for(int i=0; i<32; ++i)
        drawVSlider(vg, m_data[i], i*width()/32, 0, width()/32, height());
}

int zHarmonics::objNear(QPointF p)
{
    //TODO: perhaps account for the blank area between widgets?
    int result = p.x()*32;
    if(result >= 0 && result <= 31)
        return result;
    return -1;
}

void zHarmonics::objMove(int x, QPointF delta)
{
    m_data[x] += delta.y();
    if(m_data[x] > 1)
        m_data[x] = 1;
    if(m_data[x] < 0)
        m_data[x] = 0;

    if(window())
        window()->update();
}

void zHarmonics::objSet(int x, float val)
{
    m_data[x] = val;
    if(m_data[x] > 1)
        m_data[x] = 1;
    if(m_data[x] < 0)
        m_data[x] = 0;

    if(window())
        window()->update();
}
