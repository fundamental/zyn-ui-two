#include "layout.h"
#include <vector>
#include <cstdio>

struct BB
{
    float aspect;
    float rel;
};

struct LayoutEngine
{
    std::vector<BB> boxes;
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
bool layoutFlowHor(LayoutEngine &l, int x, int y, int w, int h)
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
        float t  = b.rel/(l.unit_cost*b.aspect)/2;
        if(t > maxH)
            maxH = t;
    }

    l.label_h  = maxH*0.3;
    l.cy_label = l.cy+(maxH+l.label_h)/2;
    if(l.cy_label+l.label_h/2 > y+h)
        return false;
    return true;
}

//Virtically Constrained Layout
//TODO centering
void layoutFlowVir(LayoutEngine &l, int x, int y, int w, int h)
{
    //constraint:
    //l.cy_label+l.label_h/2 == y+h
    //????
    //0.62maxH = h
    
    const float goal = h*0.62;
    float unit_cost = 1e-4;
    for(auto b:l.boxes) {
        float t  = b.rel/(unit_cost*b.aspect)/2;
        if(t > goal)
            unit_cost = b.rel/(goal*b.aspect)/2;
    }

    l.unit_cost = unit_cost;
    l.cy = y+h/2;
    l.x = x;

    //Find max Height To Define label row
    float maxH = 0;
    for(auto b:l.boxes) {
        float t  = b.rel/(l.unit_cost*b.aspect)/2;
        if(t > maxH)
            maxH = t;
    }

    l.label_h  = maxH*0.3;
    l.cy_label = l.cy+(maxH+l.label_h)/2;
}

void layoutFlow(layout_t layout, int x, int y, int w, int h)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(!layoutFlowHor(l, x, y, w, h))
        layoutFlowVir(l, x, y, w, h);
}

void layoutGet(layout_t layout, int elm, float *pos)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(elm >= l.boxes.size()) {
        printf("Cannot return elm %d of %d\n", elm, (int)l.boxes.size());
        return;
    }
    float x_off = l.x;
    for(int i=0; i<elm; ++i)
        x_off += l.boxes[i].rel/(l.unit_cost*l.boxes[i].aspect);
    pos[0] = l.x+x_off;
    pos[3] = l.boxes[elm].rel/l.unit_cost;
    pos[2] = pos[3]/l.boxes[elm].aspect;
    pos[1] = l.cy - pos[3]/2;
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
    return new LayoutEngine();
}

void layoutDestroy(layout_t layout)
{
    delete (LayoutEngine*)layout;
}

//[0:x, 1:y, 2:w, 3:h]
void pad(float scale, float *bb)
{
    float cx = bb[0] + bb[2]/2.0;
    float cy = bb[1] + bb[3]/2.0;
    float w  = bb[2]*scale;
    float h  = bb[3]*scale;
    bb[0] = cx - w/2.0;
    bb[1] = cy - h/2.0;
    bb[2] = w;
    bb[3] = h;
}
