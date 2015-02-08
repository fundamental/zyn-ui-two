#include "grid.h"
#include <stdlib.h>

typedef struct
{
    int rows;
    int cols;
    float w;
    float h;
} grid_struct_t;

grid_t gridCreate(int traits, int rows, int cols)
{
    grid_struct_t *g = malloc(sizeof(grid_struct_t));
    g->rows = rows;
    g->cols = cols;
    return g;
}

void gridFlow(grid_t g_, float x, float y, float w, float h)
{
    grid_struct_t *g = (grid_struct_t*)g_;
    g->w = w;
    g->h = h;
}

void gridGet(grid_t g_, int item, float *pos)
{
    grid_struct_t *g = (grid_struct_t*)g_;
    int r = item%g->cols;
    int c = item/g->cols;
    pos[0] = r*g->w/g->cols;
    pos[1] = c*g->h/g->rows;
    pos[2] = g->w/g->cols;
    pos[3] = g->h/g->rows;
}

