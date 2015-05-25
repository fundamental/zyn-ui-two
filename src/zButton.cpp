#include "zButton.h"

zButton::zButton()
    :m_t(0), m_renderer("default")
{}

void zButton::paint(NVGcontext *vg)
{
    if(m_renderer == "default") {
        drawButton(vg, getLabel().c_str(), 0, 0, width(), height());
        m_zaspect = textAspect(vg, getLabel().c_str());
    } else if(m_renderer == "relaxed") {
        drawRelaxedButton(vg, getLabel().c_str(), 0, 0, width(), height());
        m_zaspect = textAspect(vg, getLabel().c_str());
    } else if(m_renderer == "icon-stop")
        drawStopButton(vg, 0, 0, width(), height());
    else if(m_renderer == "icon-pause")
        drawPauseButton(vg, 0, 0, width(), height());
    else if(m_renderer == "icon-record")
        drawRecButton(vg, 0, 0, width(), height());
    else if(m_renderer == "icon-panic")
        drawPanicButton(vg, 0, 0, width(), height());
    else if(m_renderer == "icon-power")
        drawPowerButton(vg, 0, 0, width(), height());
    else if(m_renderer == "icon-keyboard")
        drawKeyButton(vg, 0, 0, width(), height());
}

void zButton::setAutoAspect(bool)
{
}

BBox &zButton::layoutSubProblems(LayoutProblem &prob, BBox &parent)
{
    bbox.parent = &parent;
    bbox.clear();
    auto &var = *prob.getNamedVariable("textHeight", 200);
    var = bbox.h;
    if(m_zexpandable) {
            bbox.h <= m_zaspect*bbox.w;
    } else {
        if(m_renderer == "default")
            bbox.h = m_zaspect*bbox.w;
        else
            bbox.h = bbox.w;
    }
    return bbox;
}
