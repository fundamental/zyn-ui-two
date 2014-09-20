#include "view.h"
#include "draw.h"
#include "layout.h"


void viewLFO(NVGcontext *vg, int x, int y, int h, int w)
{
    const char *knob_seq[]  = {"FREQ.", "DEPTH", "START", "DELAY"};
    const char *mini_knob[] = {"STR.", "A.R.", "F.R."};
    const char *pulldown[]  = {"TYPE"};
    const char *button[]    = {"C"};

    layout_t layout = layoutCreate();
    for(int i=0; i<sizeof(knob_seq)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1.5);
    for(int i=0; i<sizeof(mini_knob)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1);
    for(int i=0; i<sizeof(pulldown)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1.0/3.0, 1);
    for(int i=0; i<sizeof(button)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1);
    layoutFlow(layout, x,y,w,h);

    for(int i=0; i<11; ++i)
    {
        float pos[4];
        layoutGet(layout, i, pos);
        drawBox(vg, pos[0], pos[1], pos[2], pos[3]);
    }
    layoutDestroy(layout);

}
