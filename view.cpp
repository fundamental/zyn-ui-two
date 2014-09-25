#include "view.h"
#include "draw.h"
#include "layout.h"
#include "widget.h"
#include <stdio.h>

#define SPLAT(x) x[0], x[1], x[2], x[3]


void viewLFO(NVGcontext *vg, int x, int y, int w, int h)
{
    YAML::Node nil;
    Module mod(vg, nil);
    mod.inner.pad_factor = 0.9;
    mod.inner.add(new Knob(vg, nil), 1, 1.5, "FREQ.");
    mod.inner.add(new Knob(vg, nil), 1, 1.5, "DEPTH");
    mod.inner.add(new Knob(vg, nil), 1, 1.5, "START");
    mod.inner.add(new Knob(vg, nil), 1, 1.5, "DELAY");

    mod.inner.add(new Knob(vg, nil), 1, 1, "STR.");
    mod.inner.add(new Knob(vg, nil), 1, 1, "A.R.");
    mod.inner.add(new Knob(vg, nil), 1, 1, "F.R.");

    mod.inner.add(new DropDown(vg, nil), 1.0/3.0, 1, "TYPE");
    mod.inner.add(new Button(vg, nil), 1, 1, "");

    //mod.upper.add(new Button(vg, "C"), 1,1, "");
    //mod.upper.add(new Button(vg, "P"), 1,1, "");
    float pos[4] = {(float)x, (float)y, (float)w, (float)h};
    mod.draw(pos);
}


void viewFilterEnv(NVGcontext *vg, int x, int y, int w, int h)
{
    YAML::Node nil;
    Module mod(vg,nil);
    mod.inner.pad_factor = 0.9;
    mod.inner.pad_factor = 0.9;
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "A.VAL");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "A.DT");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "D.VAL");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "D.DT");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "R.DT");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "R.VAL");

    mod.inner.add(new Knob(vg,nil), 1, 1, "STR.");
    mod.inner.add(new Button(vg, nil), 1, 1, "");

    //mod.upper.add(new Button(vg, "C"), 1,1, "");
    //mod.upper.add(new Button(vg, "P"), 1,1, "");
    //mod.upper.add(new Button(vg, "EDIT"), 1.0/3.0,1, "");

    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    mod.draw(pos);
}

void viewReverb(NVGcontext *vg, int x, int y, int w, int h)
{
    YAML::Node nil;
    Module mod(vg, nil);
    mod.inner.pad_factor = 0.9;
    mod.inner.pad_factor = 0.9;
    mod.inner.add(new DropDown(vg, nil), 1.0/3.0, 1, "PRESET");
    mod.inner.add(new DropDown(vg, nil), 1.0/3.0, 1, "TYPE");
    layoutDummyBox(mod.inner.layout);
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "R.S");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "D/W");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "PAN");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "TIME");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "I.DEL");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "I.DELFB");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "LPF");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "HPF");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "DAMP");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "BW");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "E/R");

    //mod.upper.add(new Button(vg, "C"), 1,1, "");
    //mod.upper.add(new Button(vg, "P"), 1,1, "");

    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    mod.draw(pos);
}

void viewAmplitude(NVGcontext *vg, int x, int y, int w, int h)
{
    YAML::Node nil;
    Module mod(vg, nil);
    mod.inner.pad_factor = 0.9;
    mod.inner.add(new KnobDetail(vg,nil), 1, 1.5, "VOL");
    mod.inner.add(new KnobDetail(vg,nil), 1, 1.5, "V.SNS");
    mod.inner.add(new Knob(vg,nil), 1, 1.5, "PAN");
    mod.inner.add(new Button(vg,nil), 1.0/3, 1);
    mod.inner.add(new Knob(vg,nil), 1, 1, "P.STR.");
    mod.inner.add(new Knob(vg,nil), 1, 1, "P.T.");
    mod.inner.add(new Knob(vg,nil), 1, 1, "P.STC.");
    mod.inner.add(new Knob(vg,nil), 1, 1, "P.VEL.");
    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    mod.draw(pos);
}

Module *Generate(const char *, NVGcontext *);

void viewAmplitude2(NVGcontext *vg, int x, int y, int w, int h)
{
    Module *mod = Generate("amplitude", vg);
    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    if(mod)
        mod->draw(pos);
}
