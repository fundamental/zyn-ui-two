#pragma once
#include "zWidget.h"
 
class zGraph:public zWidget
{
    Q_OBJECT
    public:
        zGraph();
        void paint(NVGcontext *vg);
};
