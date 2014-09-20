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
    virtual void draw(float*);
    Group inner;
};
