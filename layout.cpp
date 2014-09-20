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
};


void layoutBoundBox(layout_t layout, float aspect, float rel)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    l.boxes.push_back(BB{aspect, rel});
}

void layoutFlow(layout_t layout, int x, int y, int w, int h)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    //Try to do a horizontal layout (XXX forgetting about virtical case here)
    float total_cost = 0;
    for(auto b:l.boxes)
        total_cost += b.rel/b.aspect;


    l.unit_cost = total_cost / w;
    l.cy = y+h/2;
    l.x = x;
}

void layoutGet(layout_t layout, int elm, float *pos)
{
    LayoutEngine &l = *(LayoutEngine*)layout;
    if(elm >= l.boxes.size())
        return;
    float x_off = l.x;
    for(int i=0; i<elm; ++i)
        x_off += l.boxes[i].rel/(l.unit_cost*l.boxes[i].aspect);
    pos[0] = l.x+x_off;
    pos[3] = l.boxes[elm].rel/l.unit_cost;
    pos[2] = pos[3]/l.boxes[elm].aspect;
    pos[1] = l.cy - pos[3]/2;
}


layout_t layoutCreate(void)
{
    return new LayoutEngine();
}

void layoutDestroy(layout_t layout)
{
    delete (LayoutEngine*)layout;
}
