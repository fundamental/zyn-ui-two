#include "layout.h"
#include <vector>
#include <cstdio>
#include <cassert>

struct BB
{
    bool expandable;
    float aspect;
    float rel;
};

struct LayoutEngine
{
    LayoutEngine(int traits_)
        :traits(traits_), expandable_id(-1), unused_space(-1),
        x(-1), cy(-1), unit_cost(-1), label_h(-1), cy_label(-1),
        vertical(!!(traits_&LAYOUT_VERT)), expandables(0)
    {}
    std::vector<BB> boxes;

    int traits;
    int expandable_id;
    bool vertical;
    float unused_space;
    float x, cy;
    float unit_cost;
    float label_h;
    float cy_label;
    int expandables;
};


void layoutBoundBox(layout_t layout, float aspect, float rel)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(l.vertical) {
        rel    = rel/aspect;
        aspect = 1.0/aspect;
    }
    l.boxes.push_back(BB{false, aspect, rel});
}

void layoutExpandable(layout_t layout)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(l.boxes.empty())
        return;
    BB &box = l.boxes[l.boxes.size()-1];
    if(box.expandable)
        return;
    box.expandable = true;
    l.expandables++;
}

void layoutDummyBox(layout_t layout)
{
    assert(false && "Out-Of-Date API");
    LayoutEngine &l = *(LayoutEngine*)layout;
    l.expandable_id = l.boxes.size();
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
        float t  = b.rel/(b.aspect*l.unit_cost);
        if(t > maxH)
            maxH = t;
    }
    l.unused_space = 0;

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
    //constraint:
    //l.cy_label+l.label_h/2 == y+h
    //????
    //0.62maxH = h
    
    const float goal = l.traits&LAYOUT_LABELS ? h*0.62 : h;
    float unit_cost = 1e-9;
    for(auto b:l.boxes) {
        float t  = b.rel/(b.aspect*unit_cost);
        if(t > goal) {
            unit_cost = b.rel/(b.aspect*goal);
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

    float used_space = 0;
    for(int i=0; i<l.boxes.size(); ++i)
        used_space += l.boxes[i].rel/(l.unit_cost*l.boxes[i].aspect);
    l.unused_space = w-used_space;
}

void layoutFlow(layout_t layout, float x, float y, float w, float h)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(l.vertical) {
        std::swap(x,y);
        std::swap(w,h);
    }
    if(!layoutFlowHor(l, x, y, w, h))
        layoutFlowVir(l, x, y, w, h);
}

static float layoutGetWidth(layout_t layout, int elm)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    const BB b = l.boxes[elm];
    float width = b.rel/(l.unit_cost*b.aspect);
    if(l.expandables) {
        width += b.expandable*l.unused_space/l.expandables;
    }
    return width;
}

void layoutGet(layout_t layout, int elm, float *pos)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    assert(l.x >= 0 && l.cy >= 0 && l.unit_cost >= 0);
    if(elm >= l.boxes.size()) {
        printf("Cannot return elm %d of %d\n", elm, (int)l.boxes.size());
        return;
    }
    float x_off = l.x;
    for(int i=0; i<elm; ++i)
        x_off += layoutGetWidth(layout, i);
    assert(x_off >= 0);
    const BB b = l.boxes[elm];
    pos[0] = x_off;
    pos[3] = b.rel/l.unit_cost;
    pos[2] = layoutGetWidth(layout, elm);
    pos[1] = l.cy - pos[3]/2.0;
    if(l.vertical) {
        std::swap(pos[0], pos[1]);
        std::swap(pos[2], pos[3]);
    }
}

void layoutGetLabel(layout_t layout, int elm, float *pos)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(elm >= l.boxes.size())
        return;
    float x_off = l.x;
    for(int i=0; i<elm; ++i) {
        if(l.expandable_id == i)
            x_off += l.unused_space;
        x_off += l.boxes[i].rel/(l.unit_cost*l.boxes[i].aspect);
    }
    if(l.expandable_id == elm)
        x_off += l.unused_space;
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

