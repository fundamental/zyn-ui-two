#pragma once
#include "zWidget.h"
#include "../draw.h"

class zLogo:public zWidget
{
    Q_OBJECT
public:
    zLogo()
    {m_label = "ZynAddSubFX";}
    void paint(NVGcontext *vg)
    {
        float h = height();
        float w = width();
        std::string tmp = getLabel();
        const char *str = tmp.c_str();
        float scale = 0.9;

        nvgFontSize(vg, scale*h);
        nvgFontFace(vg, "sans");
        nvgFillColor(vg, nvgRGBA(0xbb, 0xbb, 0xbb, 255));

        nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        float bounds[4];
        nvgTextBounds(vg, 0,0, str, NULL, bounds);
        if((bounds[2]-bounds[0]) > w) //horizontally constrained case
            nvgFontSize(vg, h*scale*w*1.0/(bounds[2]-bounds[0]));

        nvgText(vg, 0+w/2,0+h*0.5f,str, NULL);
    };
};
