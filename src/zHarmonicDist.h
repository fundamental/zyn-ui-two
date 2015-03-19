#pragma once
#include "zGraph.h"
 
class zHarmonicDist:public zGraph
{
    Q_OBJECT
    public:
        zHarmonicDist();
        void paint(NVGcontext *vg);
};
