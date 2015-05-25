#pragma once
#include "zWidget.h"
 
class zStatusPane:public zWidget
{
    Q_OBJECT
    public:
        zStatusPane();
        void paint(NVGcontext *vg);
};
