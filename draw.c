#include <math.h>
#include <stdio.h>
#include "draw.h"


static char* cpToUTF8(int cp, char* str)
{
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
	case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
	case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
	case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
	case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
	case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
	case 1: str[0] = cp;
	}
	return str;
}
#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729
#define ICON_DOWN  0xE764

void drawDial(NVGcontext *vg, int x, int y, int w, int h)
{
    int cy = y+h/2;
    int cx = x+w/2;
    int kr = h/4;
	nvgBeginPath(vg);
    nvgArc(vg, cx, cy, 0.4*h, 0, 1.0/2.0*M_PI, 1);
    nvgArc(vg, cx, cy, 0.2*h, 1.0/2.0*M_PI, 0, 2);
    nvgClosePath(vg);
    //nvgArc(NVGcontext* ctx, float cx, float cy, float r, float a0, float a1, int dir);
    //nvgClosePath(NVGcontext* ctx);
    //nvgLineTo(NVGcontext* ctx, float x, float y);
	//nvgCircle(vg, cx, cy, kr-1);
	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
	nvgFill(vg);
	

    nvgBeginPath(vg);
    nvgArc(vg, cx, cy, 0.4*h, -1.8, 0.5*M_PI, 1);
    nvgArc(vg, cx, cy, 0.2*h, 0.5*M_PI, -1.8, 2);
    nvgClosePath(vg);
	nvgFillColor(vg, nvgRGBA(0x3a,0xc5,0xec,255));
	nvgFill(vg);
}

void drawDialValue(NVGcontext *vg, int val, int x, int y, int w, int h)
{
    char str[128];
    snprintf(str, 128, "%d", val);

    int X = x+w/2;
    int Y = y+h/2;
    int W = w/2;
    int H = h/2;

	nvgFontSize(vg, H*0.6);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,128));

	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, X+W/2,Y+H*0.5f,str, NULL);
}

//------------------------------------------------------------------------------
void drawLabel(NVGcontext *vg, const char *str, int x, int y, int w, int h)
{
	nvgFontSize(vg, h*1.0);
	nvgFontFace(vg, "sans");
	//nvgFillColor(vg, nvgRGBA(0x22, 0x9b, 0xdb, 128));
	nvgFillColor(vg, nvgRGBA(0x32, 0xb7, 0xdd, 255));

	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    float bounds[4];
    nvgTextBounds(vg, x,y, str, NULL, bounds);
    if((bounds[2]-bounds[0]) > w) //horizontally constrained case
        nvgFontSize(vg, h*w*1.0/(bounds[2]-bounds[0]));

	nvgText(vg, x+w/2,y+h*0.5f,str, NULL);
}

//------------------------------------------------------------------------------
void drawLeftLabel(NVGcontext *vg, const char *str, int x, int y, int w, int h)
{

	nvgFontSize(vg, h*1.0);
	nvgFontFace(vg, "sans");
	//nvgFillColor(vg, nvgRGBA(0x22, 0x9b, 0xdb, 128));
	nvgFillColor(vg, nvgRGBA(0x32, 0xb7, 0xdd, 255));

	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);

    float bounds[4];
    nvgTextBounds(vg, x,y, str, NULL, bounds);
    if((bounds[2]-bounds[0]) > w) //horizontally constrained case
        nvgFontSize(vg, h*w*1.0/(bounds[2]-bounds[0]));


	nvgText(vg, x,y+h*0.5f,str, NULL);
}

//------------------------------------------------------------------------------

void drawBox(NVGcontext *vg, int x, int y, int w, int h)
{
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0x12,0x58, 0x5d,255));
    nvgStrokeColor(vg, nvgRGBA(0x10,0x7a,0xa3,255));
    nvgFill(vg);
    nvgStroke(vg);
}

//------------------------------------------------------------------------------

void drawButtonScale(NVGcontext *vg, float scale, const char *str, int x, int y,
        int w, int h)
{
    drawBox(vg, x,y,w,h);


	nvgFontSize(vg, h*scale);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(0x22, 0x9b, 0xdb, 255));

	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    float bounds[4];
    nvgTextBounds(vg, x,y, str, NULL, bounds);
    if((bounds[2]-bounds[0]) > w) //horizontally constrained case
        nvgFontSize(vg, h*scale*w*1.0/(bounds[2]-bounds[0]));

	nvgText(vg, x+w/2,y+h*0.5f,str, NULL);
}
//------------------------------------------------------------------------------

void drawButton(NVGcontext *vg, const char *str, int x, int y, int w, int h)
{
    drawButtonScale(vg, 0.6, str, x,y,w,h);
}

//------------------------------------------------------------------------------
void drawOptButton(NVGcontext *vg, const char *str, int x, int y, int w, int h)
{
    char icon[8];

    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0x12,0x58, 0x5d,255));
    nvgStrokeColor(vg, nvgRGBA(0x10,0x7a,0xa3,255));
    nvgFill(vg);
    nvgStroke(vg);


	nvgFontSize(vg, h*0.6);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(0x22, 0x9b, 0xdb, 255));

	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+(w-h)/2,y+h*0.5f,str, NULL);
	
    nvgFontSize(vg, h*0.6);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,128));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h/2,y+h*0.5f,cpToUTF8(ICON_DOWN,icon), NULL);
}

//------------------------------------------------------------------------------
void drawButtonGrid(NVGcontext *vg, int n, int m, int x, int y, int w, int h)
{
    float W=w*1.0/n;
    float H=h*1.0/m;

    int cnt = 0;
    for(int i=0; i<n; ++i) {
        for(int j=0; j<m; ++j) {
            int xx = x+0.05*W + W*i;
            int yy = y+0.05*H + H*j;
            drawButtonScale(vg, 0.9, "0", xx, yy, 0.9*W, 0.9*H);
        }
    }
}

//------------------------------------------------------------------------------
void drawGrid(NVGcontext *vg, int n, int m, int x, int y, int w, int h)
{
    float W=w*1.0/n;
    float H=h*1.0/m;

    int cnt = 0;
    for(int i=0; i<=n; ++i) {
        nvgBeginPath(vg);
        nvgMoveTo(vg, i*W+x, y);
        nvgLineTo(vg, i*W+x, y+h);
        nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
        nvgStroke(vg);
    }
    for(int i=0; i<=m; ++i) {
        nvgBeginPath(vg);
        nvgMoveTo(vg, x,   y+i*H);
        nvgLineTo(vg, x+w, y+i*H);
        nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
        nvgStroke(vg);
    }
}
//------------------------------------------------------------------------------
void drawSin(NVGcontext *vg, int x, int y, int w, int h)
{
    int N = 128;
    float W=w*1.0/N;

    nvgBeginPath(vg);
    {
        int i=0;
        nvgMoveTo(vg, x+i*W, y+h/2-h*0.5*sin(2*M_PI*i*1.0/(N-1)));
    }
    for(int i=0; i<N; ++i) {
        nvgLineTo(vg, x+i*W, y+h/2-h*0.5*sin(2*M_PI*i*1.0/(N-1)));
    }

    nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
    nvgStroke(vg);
}
//------------------------------------------------------------------------------
void drawAltDial(NVGcontext *vg, int x, int y, int w, int h)
{
    float start = M_PI/4;
    float end   = M_PI*3.0/4.0;
    int cy = y+h/2;
    int cx = x+w/2;
    int kr = h/4;
	nvgBeginPath(vg);
    nvgArc(vg, cx, cy, 0.4*h, start, end, 1);
    nvgArc(vg, cx, cy, 0.2*h, end, start, 2);
    nvgClosePath(vg);
    //nvgArc(NVGcontext* ctx, float cx, float cy, float r, float a0, float a1, int dir);
    //nvgClosePath(NVGcontext* ctx);
    //nvgLineTo(NVGcontext* ctx, float x, float y);
	//nvgCircle(vg, cx, cy, kr-1);
	//nvgFillColor(vg, nvgRGBA(0x3a,0xc5,0xec,255));
	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
	nvgFill(vg);
	

    const float len = (3.0/2.0*M_PI)*0.3;
    float startt = end + len;

    nvgBeginPath(vg);
    nvgArc(vg, cx, cy, 0.2*h, end, startt, 2);
    nvgArc(vg, cx, cy, 0.4*h, startt, end, 1);
    nvgClosePath(vg);
	nvgFillColor(vg, nvgRGBA(0x3a,0xc5,0xec,255));
	nvgFill(vg);

}

//------------------------------------------------------------------------------
void drawEnvEdit(NVGcontext *vg, float *dat, int n, int m, int x, int y, int w, int h)
{
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0x0d,0x0d,0x0d,255));
    nvgStrokeColor(vg, nvgRGBA(0x01, 0x47, 0x67,255));
    nvgFill(vg);
    nvgStroke(vg);


    //Draw UnderLine
    ////Bottom Half
    nvgScissor(vg, x,y+h/2,w,h/2);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x,y);
    for(int i=0; i<n; ++i)
        nvgLineTo(vg, x+w*dat[2*i+1], y+h/2-h/2*dat[2*i]);
    nvgLineTo(vg, x+w, y);
    nvgClosePath(vg);
	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,55));
    nvgFill(vg);
    nvgResetScissor(vg);
    ////Upper Half
    nvgScissor(vg, x,y,w,h/2);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x,y+h);
    for(int i=0; i<n; ++i)
        nvgLineTo(vg, x+w*dat[2*i+1], y+h/2-h/2*dat[2*i]);
    nvgMoveTo(vg, x+w, y+h);
    nvgClosePath(vg);
	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,55));
    nvgFill(vg);
    nvgResetScissor(vg);

    //Draw Zero Line
    nvgBeginPath(vg);
    nvgMoveTo(vg, x, y+h/2);
    nvgLineTo(vg, x+w, y+h/2);
	nvgStrokeColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    nvgStroke(vg);

    //Draw Sel Line
    if(m >= 0 && m <n) {
        nvgBeginPath(vg);
        nvgMoveTo(vg, x+w*dat[2*m+1], y);
        nvgLineTo(vg, x+w*dat[2*m+1], y+h);
        nvgStrokeColor(vg, nvgRGBA(0x11,0x45,0x75,255));
        nvgStroke(vg);
    }

    //Draw Actual Line
    nvgBeginPath(vg);
    nvgMoveTo(vg, x+w*dat[1], y+h-h*dat[0]);
    for(int i=0; i<n; ++i)
        nvgLineTo(vg, x+w*dat[2*i+1], y+h/2-h/2*dat[2*i]);
    nvgStrokeWidth(vg, 4);
	nvgStrokeColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    nvgStroke(vg);
    nvgStrokeWidth(vg, 1);

    //Draw Grabbable Points
    for(int i=0; i<n; ++i)  {
        float xx = x+w*dat[2*i+1];
        float yy = y+h/2-h/2*dat[2*i];
        nvgBeginPath(vg);
        nvgRect(vg, xx-5,yy-5,10,10);
        nvgFillColor(vg, nvgRGBA(0,0,0,255));
        nvgStrokeColor(vg, nvgRGBA(0x10,0x7a,0xa3,255));
        nvgFill(vg);
        nvgStroke(vg);
    }

}
//------------------------------------------------------------------------------
void drawHZSlider(NVGcontext *vg, int x, int y, int w, int h)
{
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0x0d,0x0d,0x0d,255));
    nvgFill(vg);

    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    boarder(0.1*h, pos);
    float cx = x+w/2;

    //fill color
    nvgBeginPath(vg);
    nvgRect(vg, cx,pos[1],pos[2]/3.0,pos[3]);
    nvgFillColor(vg, nvgRGBA(0xb, 0x2b, 0x4d,255));
    nvgFill(vg);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void renderDial(NVGcontext *vg, dial_t dial)
{
    drawDial(vg, dial.x, dial.y,dial.w,dial.w);
    drawDialValue(vg, dial.val, dial.x, dial.y, dial.w, dial.w);
    drawLabel(vg, dial.label,dial.x, dial.y+dial.w, dial.w, dial.w/3);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
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
