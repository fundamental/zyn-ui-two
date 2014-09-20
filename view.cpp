#include "view.h"
#include "draw.h"
#include "layout.h"
#include "widget.h"
#include <stdio.h>

#define SPLAT(x) x[0], x[1], x[2], x[3]


void viewLFOInner(NVGcontext *vg, int x, int y, int w, int h)
{
    Group g(vg);
    g.pad_factor = 0.9;
    g.add(new Knob(vg), 1, 1.5, "FREQ.");
    g.add(new Knob(vg), 1, 1.5, "DEPTH");
    g.add(new Knob(vg), 1, 1.5, "START");
    g.add(new Knob(vg), 1, 1.5, "DELAY");

    g.add(new Knob(vg), 1, 1, "STR.");
    g.add(new Knob(vg), 1, 1, "A.R.");
    g.add(new Knob(vg), 1, 1, "F.R.");

    g.add(new DropDown(vg, "TEXT"), 1.0/3.0, 1, "TYPE");
    g.add(new Button(vg, "C"), 1, 1, "");

    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    g.draw(pos);
}

void viewLFO(NVGcontext *vg, int x, int y, int w, int h)
{
    //Paint the Whole thing black
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgFill(vg);
    ////paint the top half
    float pos[4] = {(float)x, (float)y, (float)w, (float)h*0.2f};
    nvgBeginPath(vg);
    boarder(h*0.01, pos);
    nvgRect(vg, SPLAT(pos));
	nvgFillColor(vg, nvgRGBA(0xa, 0x2e, 0x4c, 255));
    nvgFill(vg);

    {
        float pos2[4] = {SPLAT(pos)};
        pos2[2] /= 3;
        pad(0.9, pos2);
        drawLeftLabel(vg, "FREQUENCY LFO", SPLAT(pos2));
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
    viewLFOInner(vg, BOARDER((h*0.01),x, y+0.2*h, w, h*0.8));
}

void viewFilterEnvInner(NVGcontext *vg, int x, int y, int w, int h)
{
    const char *knob_seq[]  = {"A.VAL", "A.DT", "D.VAL", "D.DT", "R.DT", "R.VAL"};
    const char *mini_knob[] = {"STR."};
    const char *button[]    = {"FRCR"};
    
    Group g(vg);
    g.pad_factor = 0.9;
    g.add(new Knob(vg), 1, 1.5, "A.VAL");
    g.add(new Knob(vg), 1, 1.5, "A.DT");
    g.add(new Knob(vg), 1, 1.5, "D.VAL");
    g.add(new Knob(vg), 1, 1.5, "D.DT");
    g.add(new Knob(vg), 1, 1.5, "R.DT");
    g.add(new Knob(vg), 1, 1.5, "R.VAL");

    g.add(new Knob(vg), 1, 1, "STR.");
    g.add(new Button(vg, "FRCR"), 1, 1, "");

    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    g.draw(pos);
}


void viewFilterEnv(NVGcontext *vg, int x, int y, int w, int h)
{
    //Paint the Whole thing black
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgFill(vg);
    ////paint the top half
    float pos[4] = {(float)x, (float)y, (float)w,(float) h*0.2f};
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
