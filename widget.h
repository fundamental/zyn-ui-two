#include <vector>
#include <string>
#include "nanovg/nanovg.h"
#include "layout.h"
#include "draw.h"
using std::string;
using std::vector;

struct Widget
{
    Widget(NVGcontext *vg)
        :vg(vg) {}
    virtual void draw(float*){};
    NVGcontext *vg;
};

struct Parent:public Widget
{
    Parent(NVGcontext *vg)
        :Widget(vg) {}
    std::vector<Widget*> children;
    std::vector<string>  labels;
    ~Parent(void) { for(auto w:children) delete w; }
};

struct Knob:public Widget
{
    Knob(NVGcontext *vg)
        :Widget(vg) {}
    virtual void draw(float *pos)
    {
        drawAltDial(vg, SPLAT(pos));
    }
};

struct DropDown:public Widget
{
    DropDown(NVGcontext *vg, std::string text_)
        :Widget(vg), text(text_) {}
    virtual void draw(float*pos)
    {
        drawOptButton(vg, text.c_str(), SPLAT(pos));
    };

    string text;
};

struct Button:public Widget
{
    Button(NVGcontext *vg, string label_)
        :Widget(vg), label(label_) {}
    virtual void draw(float *pos)
    {
        drawButton(vg, label.c_str(), SPLAT(pos));
    }
    string label;
};

//Assumes simple 1D Flow right now
struct Group:public Parent
{
    float pad_factor;
    layout_t layout;


    Group(NVGcontext *vg)
        :Parent(vg),pad_factor(1.0), layout(layoutCreate(LAYOUT_LABELS)){}
    ~Group(void) {layoutDestroy(layout);}

    virtual void draw(float *pos)
    {
        layoutFlow(layout, SPLAT(pos));
        int N = children.size();
        for(int i=0; i<N; ++i) {
            float pos2[4];
            layoutGet(layout, i, pos2);
            pad(pad_factor, pos2);
            children[i]->draw(pos2);
            if(!labels[i].empty()) {
                layoutGetLabel(layout, i, pos2);
                pad(pad_factor, pos2);
                drawLabel(vg, labels[i].c_str(), SPLAT(pos2));
            }
        }
    }

    void add(Widget *w, float aspect, float weight, string label="")
    {
        children.push_back(w);
        labels.push_back(label);
        layoutBoundBox(layout, aspect, weight);
    }

    void flow(float x, float y, float w, float h)
    {
        layoutFlow(layout, x,y,w,h);
    }
};
struct Module:public Parent
{
    string label;
    Module(NVGcontext *vg, string label_)
        :Parent(vg), inner(vg), upper(vg), label(label_) {}
    virtual void draw(float*pos_)
    {
        //Paint the Whole thing black
        nvgBeginPath(vg);
        nvgRect(vg, SPLAT(pos_));
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
        nvgFill(vg);

        {
            ////paint the top half
            float pos[4] = {SPLAT(pos_)};
            pos[3] *= 0.2;
            nvgBeginPath(vg);
            boarder(pos_[3]*0.01, pos);
            nvgRect(vg, SPLAT(pos));
            nvgFillColor(vg, nvgRGBA(0xa, 0x2e, 0x4c, 255));
            nvgFill(vg);
            float upperspace[4] = {pos[0]+pos[2]*2.0f/3.0f,
                pos[1], pos[2]/3.0f, pos[3]};
            upper.draw(upperspace);


            float pos2[4] = {SPLAT(pos)};
            pos2[2] /= 3;
            pad(0.9, pos2);
            drawLeftLabel(vg, label.c_str(), SPLAT(pos2));
        }



        {
            SMAP(pos_);
            float innerspace[4] = {x, y+h*0.2f, w, h*0.8f};
            boarder(h*0.01, innerspace);
            //paint the inner panel
            nvgBeginPath(vg);
            nvgRect(vg, SPLAT(innerspace));
            nvgFillColor(vg, nvgRGBA(0x6, 0x27, 0x37, 255));
            nvgFill(vg);
            inner.draw(innerspace);
        }
    }
    Group upper;
    Group inner;
};
