#include "zTextField.h"

zTextField::zTextField()
{}

void zTextField::paint(NVGcontext *vg)
{
    const char *label = "TextField";
    if(!m_label.isEmpty())
        label = m_label.toLatin1().data();
    //drawButton(vg, label, 0, 0, width(), height());
    float scale = 0.8;
    nvgFontSize(vg, height()*scale);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(0x00, 0xcf, 0xf7, 255));

    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    float bounds[4];
    nvgTextBounds(vg, 0, 0, label, NULL, bounds);
    if((bounds[2]-bounds[0]) > width()) //horizontally constrained case
        nvgFontSize(vg, height()*scale*width()*1.0/(bounds[2]-bounds[0]));

    nvgText(vg, width()/2,height()*0.5f,label, NULL);
}
