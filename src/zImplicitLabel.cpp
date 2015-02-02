#include "zImplicitLabel.h"

zImplicitLabel::zImplicitLabel(QQuickItem *parent)
    :zWidget(parent)
{}

void zImplicitLabel::paint(NVGcontext *vg)
{
    drawLabel(vg, getLabel().c_str(), 0, 0, width(), height());
}
