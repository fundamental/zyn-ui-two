#include "layout.h"
#include <vector>
#include <cstdio>
#include <cassert>

struct BB
{
    float aspect;
    float rel;
};

struct LayoutEngine
{
    LayoutEngine(int traits_)
        :traits(traits_), x(-1), cy(-1), unit_cost(-1), label_h(-1), cy_label(-1)
    {}
    std::vector<BB> boxes;
    int traits;
    float x, cy;
    float unit_cost;
    float label_h;
    float cy_label;
};


void layoutBoundBox(layout_t layout, float aspect, float rel)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    l.boxes.push_back(BB{aspect, rel});
}

//returns false on unfeasible layout
bool layoutFlowHor(LayoutEngine &l, float x, float y, float w, float h)
{
    //Try to do a horizontal layout
    float total_cost = 0;
    for(auto b:l.boxes)
        total_cost += b.rel/b.aspect;


    l.unit_cost = total_cost / w;
    l.cy = y+h/2;
    l.x = x;

    //Find max Height To Define label row
    float maxH = 0;
    for(auto b:l.boxes) {
        float t  = b.rel*b.aspect/l.unit_cost;
        if(t > maxH)
            maxH = t;
    }

    if(l.traits & LAYOUT_LABELS) {
        l.label_h  = maxH*0.3;
        l.cy_label = l.cy+(maxH+l.label_h)/2;
        if(l.cy_label+l.label_h/2 > y+h)
            return false;
    } else
        return maxH < h;

    return true;
}

//Virtically Constrained Layout
//TODO centering
void layoutFlowVir(LayoutEngine &l, float x, float y, float w, float h)
{
    //printf("LayoutFlowVir\n");
    //constraint:
    //l.cy_label+l.label_h/2 == y+h
    //????
    //0.62maxH = h
    
    const float goal = l.traits&LAYOUT_LABELS ? h*0.62 : h;
    float unit_cost = 1e-9;
    for(auto b:l.boxes) {
        //printf("%f = [%f /(%f %f)]\n", goal,  b.rel, unit_cost, b.aspect);
        float t  = b.rel*b.aspect/unit_cost;
        if(t > goal) {
            unit_cost = b.rel*b.aspect/goal;
            //printf("Update unit cost = %f\n", unit_cost);
        }
    }

    l.unit_cost = unit_cost;
    l.cy = y+h/2;
    l.x = x;

    //Find max Height To Define label row
    float maxH = 0;
    for(auto b:l.boxes) {
        float t  = b.rel*b.aspect/l.unit_cost;
        if(t > maxH)
            maxH = t;
    }

    if(l.traits&LAYOUT_LABELS) {
        l.label_h  = maxH*0.3;
        l.cy_label = l.cy+(maxH+l.label_h)/2;
    }

}

void layoutFlow(layout_t layout, float x, float y, float w, float h)
{
    //printf("layoutflow(%f,%f,%f,%f)\n", x,y,w,h);
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(!layoutFlowHor(l, x, y, w, h))
        layoutFlowVir(l, x, y, w, h);
}

void layoutGet(layout_t layout, int elm, float *pos)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    assert(l.x >= 0 && l.cy >= 0 && l.unit_cost >= 0);
    ////printf("lx = %f\n", l.x);
    //printf("lcy = %f\n", l.cy);
    //printf("lunit = %f\n", l.unit_cost);
    if(elm >= l.boxes.size()) {
        printf("Cannot return elm %d of %d\n", elm, (int)l.boxes.size());
        return;
    }
    float x_off = l.x;
    for(int i=0; i<elm; ++i)
        x_off += l.boxes[i].rel/(l.unit_cost*l.boxes[i].aspect);
    //printf("%f/%f = %f\n", l.boxes[elm].aspect, l.unit_cost, l.boxes[elm].aspect/ l.unit_cost);
    //printf("%f - %f/2 = %f\n", l.cy, l.boxes[elm].aspect/ l.unit_cost, l.cy - l.boxes[elm].aspect/ l.unit_cost/2);
    assert(x_off >= 0);
    pos[0] = x_off;
    pos[3] = l.boxes[elm].rel/l.unit_cost;
    pos[2] = l.boxes[elm].rel/(l.unit_cost*l.boxes[elm].aspect);
    pos[1] = l.cy - pos[3]/2.0;
    //printf("res = [%f %f %f %f]\n", pos[0], pos[1], pos[2], pos[3]);
}

void layoutGetLabel(layout_t layout, int elm, float *pos)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(elm >= l.boxes.size())
        return;
    float x_off = l.x;
    for(int i=0; i<elm; ++i)
        x_off += l.boxes[i].rel/(l.unit_cost*l.boxes[i].aspect);
    pos[0] = l.x+x_off;
    pos[3] = l.label_h;
    pos[2] = l.boxes[elm].rel/(l.unit_cost*l.boxes[elm].aspect);
    pos[1] = l.cy_label - pos[3]/2;
}


layout_t layoutCreate(int traits)
{
    return new LayoutEngine(traits);
}

void layoutDestroy(layout_t layout)
{
    delete (LayoutEngine*)layout;
}

//[0:x, 1:y, 2:w, 3:h]
float *pad(float scale, float *bb)
{
    float cx = bb[0] + bb[2]/2.0;
    float cy = bb[1] + bb[3]/2.0;
    float w  = bb[2]*scale;
    float h  = bb[3]*scale;
    bb[0] = cx - w/2.0;
    bb[1] = cy - h/2.0;
    bb[2] = w;
    bb[3] = h;
    return bb;
}

float *boarder(float scale, float *bb)
{
    bb[0] += scale;
    bb[1] += scale;
    bb[2] -= 2*scale;
    bb[3] -= 2*scale;
    return bb;
}
