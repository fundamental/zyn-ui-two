#include "view.h"
#include "draw.h"
#include "layout.h"
#include <stdio.h>

#define SPLAT(x) x[0], x[1], x[2], x[3]


void viewLFO(NVGcontext *vg, int x, int y, int w, int h)
{
    const char *knob_seq[]  = {"FREQ.", "DEPTH", "START", "DELAY"};
    const char *mini_knob[] = {"STR.", "A.R.", "F.R."};
    const char *pulldown[]  = {"TYPE"};
    const char *button[]    = {"C"};

    layout_t layout = layoutCreate(LAYOUT_LABELS);
    for(int i=0; i<sizeof(knob_seq)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1.5);
    for(int i=0; i<sizeof(mini_knob)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1);
    for(int i=0; i<sizeof(pulldown)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1.0/3.0, 1);
    for(int i=0; i<sizeof(button)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1);
    layoutFlow(layout, x,y,w,h);

#if 1
    //Bounding Box View
    for(int i=0; i<9; ++i)
    {
        float pos[4];
        layoutGet(layout, i, pos);
        //drawBox(vg, pos[0], pos[1], pos[2], pos[3]);
        //pad(0.9, pos);
        drawBox(vg, pos[0], pos[1], pos[2], pos[3]);
    }
    for(int i=0; i<9; ++i)
    {
        float pos[4];
        layoutGetLabel(layout, i, pos);
        //drawBox(vg, pos[0], pos[1], pos[2], pos[3]);
        //pad(0.9, pos);
        drawButton(vg, "text", pos[0], pos[1], pos[2], pos[3]);
    }
#else
    float pos[4];
    for(int i=0; i<7; ++i) {
        layoutGet(layout, i, pos);
        drawAltDial(vg, SPLAT(pos));
    }
    layoutGet(layout, 7, pos);
    pad(0.8, pos);
    drawOptButton(vg, "TEXT", SPLAT(pos));
    layoutGet(layout, 8, pos);
    pad(0.8, pos);
    drawButton(vg, "C", SPLAT(pos));
    
    for(int i=0; i<4; ++i)
    {
        layoutGetLabel(layout, i, pos);
        pad(0.9, pos);
        drawLabel(vg, knob_seq[i], SPLAT(pos));
    }
    for(int i=4; i<7; ++i)
    {
        layoutGetLabel(layout, i, pos);
        pad(0.9, pos);
        drawLabel(vg, mini_knob[i-4], SPLAT(pos));
    }

    layoutGetLabel(layout, 7, pos);
    pad(0.9, pos);
    drawLabel(vg, pulldown[0], SPLAT(pos));
#endif



    layoutDestroy(layout);
}

void viewFilterEnvInner(NVGcontext *vg, int x, int y, int w, int h)
{
    const char *knob_seq[]  = {"A.VAL", "A.DT", "D.VAL", "D.DT", "R.DT", "R.VAL"};
    const char *mini_knob[] = {"STR."};
    const char *button[]    = {"FRCR"};

    layout_t layout = layoutCreate(LAYOUT_LABELS);
    for(int i=0; i<sizeof(knob_seq)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1.5);
    for(int i=0; i<sizeof(mini_knob)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1.0/3.0, 1);
    for(int i=0; i<sizeof(button)/sizeof(void*); ++i)
        layoutBoundBox(layout, 1, 1);
    layoutFlow(layout, x,y,w,h);

    float pos[4];
    for(int i=0; i<6; ++i) {
        layoutGet(layout, i, pos);
        drawAltDial(vg, SPLAT(pos));
    }
    layoutGet(layout, 6, pos);
    pad(0.8, pos);
    drawOptButton(vg, "TEXT", SPLAT(pos));
    layoutGet(layout, 7, pos);
    pad(0.8, pos);
    drawButton(vg, button[0], SPLAT(pos));
    
    for(int i=0; i<6; ++i)
    {
        layoutGetLabel(layout, i, pos);
        pad(0.9, pos);
        drawLabel(vg, knob_seq[i], SPLAT(pos));
    }
    for(int i=6; i<7; ++i)
    {
        layoutGetLabel(layout, i, pos);
        pad(0.9, pos);
        drawLabel(vg, mini_knob[i-4], SPLAT(pos));
    }

    //layoutGetLabel(layout, 7, pos);
    //pad(0.9, pos);
    //drawLabel(vg, pulldown[0], SPLAT(pos));



    layoutDestroy(layout);
}


void viewFilterEnv(NVGcontext *vg, int x, int y, int w, int h)
{
    //Paint the Whole thing black
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgFill(vg);
    ////paint the top half
    float pos[4] = {x, y, w, h*0.2};
    nvgBeginPath(vg);
    boarder(h*0.01, pos);
    nvgRect(vg, SPLAT(pos));
	nvgFillColor(vg, nvgRGBA(0xa, 0x2e, 0x4c, 255));
    nvgFill(vg);

    {
        float pos2[4] = {SPLAT(pos)};
        pos2[2] /= 3;
        pad(0.9, pos2);
        drawLeftLabel(vg, "AMPLITUDE ENVELOPE", SPLAT(pos2));
    }

    //Do a layout of <1,1,3>
    layout_t layout = layoutCreate(LAYOUT_NONE);
    layoutBoundBox(layout, 1,     1);
    layoutBoundBox(layout, 1,     1);
    layoutBoundBox(layout, 1/3.0, 1);
    layoutFlow(layout, x+w*2.0/3.0, y, w/3.0, h*0.2);
    //printf("Within    [%d %d %d %d]{%f}\n", x,y,w,h, x+w*2.0/3.0);
    for(int i=0; i<3; ++i)
    {
        float pos[4];
        layoutGet(layout, i, pos);
        //drawBox(vg, pos[0], pos[1], pos[2], pos[3]);
        //printf("OVER HERE [%f %f %f %f]\n", SPLAT(pos));
        pad(0.9, pos);
        drawBox(vg, SPLAT(pos));
    }
    layoutDestroy(layout);

    

    //paint the inner panel
    nvgBeginPath(vg);
    nvgRect(vg, BOARDER((h*0.01), x, y+0.2*h, w, h*0.8));
	nvgFillColor(vg, nvgRGBA(0x6, 0x27, 0x37, 255));
    nvgFill(vg);
    viewFilterEnvInner(vg, BOARDER((h*0.01),x, y+0.2*h, w, h*0.8));
}
