#include "nanovg/nanovg.h"
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define SCALE(v,x,y,w,h) x + (1-v)*w/2, y + (1-v)*h/2, v*w, v*h
#define BOARDER(v,x,y,w,h) x+v,y+v, w-2*v, h-2*v

#ifndef SPLAT
#define SPLAT(x) x[0], x[1], x[2], x[3]
#endif

void drawDial(NVGcontext *vg, int x, int y, int w, int h);
void drawLabel(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawLeftLabel(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawDialValue(NVGcontext *vg, int val, int x, int y, int w, int h);
void drawBox(NVGcontext *vg, int x, int y, int w, int h);
void drawButton(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawOptButton(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawButtonGrid(NVGcontext *vg, int n, int m, int x, int y, int w, int h);
void drawGrid(NVGcontext *vg, int n, int m, int x, int y, int w, int h);
void drawSin(NVGcontext *vg, int x, int y, int w, int h);
void drawAltDial(NVGcontext *vg, int x, int y, int w, int h);
void drawEnvEdit(NVGcontext *vg, float *dat, int n, int m, int x, int y, int w, int h);
void drawHZSlider(NVGcontext *vg, int x, int y, int w, int h);
;;;;;;

typedef struct
{
    int val;
    int x, y, w;
    const char *label;
} dial_t;

void renderDial(NVGcontext *vg, dial_t dial);

//General Geometry
float *pad(float scale, float *bb);
float *boarder(float scale, float *bb);
#ifdef __cplusplus
}
#endif
