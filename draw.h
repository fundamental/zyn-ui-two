#include "nanovg/nanovg.h"

void drawDial(NVGcontext *vg, int x, int y, int w, int h);
void drawLabel(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawDialValue(NVGcontext *vg, int val, int x, int y, int w, int h);
void drawBox(NVGcontext *vg, int x, int y, int w, int h);
void drawButton(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawOptButton(NVGcontext *vg, const char *str, int x, int y, int w, int h);
void drawButtonGrid(NVGcontext *vg, int n, int m, int x, int y, int w, int h);
void drawGrid(NVGcontext *vg, int n, int m, int x, int y, int w, int h);
void drawSin(NVGcontext *vg, int x, int y, int w, int h);
void drawAltDial(NVGcontext *vg, int x, int y, int w, int h);
;;;;;;

typedef struct
{
    int val;
    int x, y, w;
    const char *label;
} dial_t;

void renderDial(NVGcontext *vg, dial_t dial);
