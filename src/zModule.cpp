#include "zModule.h"
#include "../draw.h"

void zModule::paint(NVGcontext *vg)
{
    drawModuleBox(vg, getLabel().c_str(), 0, 0, width(), height());
}
