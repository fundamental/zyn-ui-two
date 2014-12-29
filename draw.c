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
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,128));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h/2,y+h*0.5f,cpToUTF8(ICON_DOWN,icon), NULL);
	

    nvgFontSize(vg, h*0.6);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(0x22, 0x9b, 0xdb, 255));

	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    float bounds[4];
    nvgTextBounds(vg, x,y, str, NULL, bounds);
    if((bounds[2]-bounds[0]) > w-h-h/4) //horizontally constrained case
        nvgFontSize(vg, h*(w-h-h/4)*0.6/(bounds[2]-bounds[0]));
	nvgText(vg, x+h/4+(w-h)/2,y+h*0.5f,str, NULL);
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
void drawAltDial(NVGcontext *vg, float val, int x, int y, int w, int h)
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
	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
	nvgFill(vg);
	

    const float len = (3.0/2.0*M_PI)*val;//0.3;
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
void drawLinx(NVGcontext *vg, float xmin, float xmax, float x, float y, float w, float h)
{
    //1,2,3,4,5,6,7,8,9,10,20
    for(int xx=xmin; xx<=(int)xmax; ++xx) {
        float base = (xx-xmin)/(xmax-xmin);
        //for(int shift=0; shift<10; ++shift) {
            //float delta = log10((shift+1)*1.0)/(xmax_-xmin_);
        float dy = h*base;
        if(dy < 0 || dy > h)
            continue;
        nvgBeginPath(vg);
        nvgMoveTo(vg, x,   y+dy);
        nvgLineTo(vg, x+w, y+dy);
        nvgStrokeColor(vg, nvgRGBA(0x01, 0x47, 0x67,255));
        nvgStroke(vg);
    }
}
//------------------------------------------------------------------------------
void drawLogx(NVGcontext *vg, float xmin, float xmax, float x, float y, float w, float h)
{
    float xmin_ = logf(xmin),
          xmax_ = logf(xmax);
    //1,2,3,4,5,6,7,8,9,10,20
    for(int xx=xmin_; xx<=(int)xmax_; ++xx) {
        float base = (xx-xmin_)/(xmax_-xmin_);
        for(int shift=0; shift<10; ++shift) {
            float delta = log10((shift+1)*1.0)/(xmax_-xmin_);
            float dy = h*(base+delta);
            if(dy < 0 || dy > h)
                continue;
            nvgBeginPath(vg);
            nvgMoveTo(vg, x,   y+dy);
            nvgLineTo(vg, x+w, y+dy);
            nvgStrokeColor(vg, nvgRGBA(0x01, 0x47, 0x67,255));
            nvgStroke(vg);
        }
    }
}
//------------------------------------------------------------------------------
void drawLogy(NVGcontext *vg, float ymin, float ymax, float x, float y, float w, float h)
{
    float ymin_ = logf(ymin),
          ymax_ = logf(ymax);
    for(int yy=ymin_; yy<=(int)ymax_; ++yy) {
        float base = (yy-ymin_)/(ymax_-ymin_);
        for(int shift=0; shift<10; ++shift) {
            float delta = log10((shift+1)*1.0)/(ymax_-ymin_);
            float dx = w*(base+delta);
            if(dx < 0 || dx > w)
                continue;
            nvgBeginPath(vg);
            nvgMoveTo(vg, x+dx, y);
            nvgLineTo(vg, x+dx, y+h);
            nvgStrokeColor(vg, nvgRGBA(0x01, 0x47, 0x67,255));
            nvgStroke(vg);
        }
    }
}
//------------------------------------------------------------------------------
void drawLogLogGrid(NVGcontext *vg, float xmin, float xmax, float ymin, float ymax, float x, float y, float w, float h)
{
    drawLogy(vg,xmin,xmax,x,y,w,h);
    drawLogy(vg,ymin,ymax,x,y,w,h);
}
//------------------------------------------------------------------------------
void drawSemiLogy(NVGcontext *vg, float xmin, float xmax, float ymin, float ymax, float x, float y, float w, float h)
{
    drawLinx(vg,xmin,xmax,x,y,w,h);
    drawLogy(vg,ymin,ymax,x,y,w,h);
}
//------------------------------------------------------------------------------
void drawEqGrid(NVGcontext *vg, float *dat, int n, int x, int y, int w, int h)
{
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0x0d,0x0d,0x0d,255));
    nvgStrokeColor(vg, nvgRGBA(0x01, 0x47, 0x67,255));
    nvgFill(vg);
    nvgStroke(vg);

    drawSemiLogy(vg, 10,60,2,5000,x,y,w,h);

    //Draw UnderLine
    nvgBeginPath(vg);
    nvgMoveTo(vg, x,y+h);
    for(int i=0; i<n; ++i) {
        float dx = w*(i*1.0/(n-1));
        nvgLineTo(vg, x+dx, y+h/2-h/2*dat[i]);
    }
    nvgLineTo(vg, x+w, y+h);
    nvgClosePath(vg);
	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,100));
    nvgFill(vg);

    //Draw Actual Line
    nvgBeginPath(vg);
    nvgMoveTo(vg, x+w*dat[1], y+h-h*dat[0]);
    for(int i=0; i<n; ++i) {
        float dx = w*(i*1.0/(n-1));
        nvgLineTo(vg, x+dx, y+h/2-h/2*dat[i]);
    }
    nvgStrokeWidth(vg, 4);
	nvgStrokeColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    nvgStroke(vg);
    nvgStrokeWidth(vg, 1);
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
void drawVZSlider(NVGcontext *vg, int x, int y, int w, int h)
{
    nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(0x0d,0x0d,0x0d,255));
    nvgFill(vg);

    float pos[4] = {(float)x,(float)y,(float)w,(float)h};
    boarder(0.1*w, pos);
    float cy = y+h/2;

    //fill color
    nvgBeginPath(vg);
    nvgRect(vg, pos[0], cy ,pos[2],pos[3]/3.0);
	nvgFillColor(vg, nvgRGBA(0x3a,0xc5,0xec,255));
    nvgFill(vg);
}
//------------------------------------------------------------------------------
void drawOscArray(NVGcontext *vg, int x, int y, int w, int h)
{
    for(int i=0; i<10; ++i)
    {
        //Draw BackGround
        {
            float pos[4] = {30.0f*i, 0.0f, 30.0f, 270.0f};
            boarder(2, pos);
            nvgBeginPath(vg);
            nvgRect(vg, SPLAT(pos));
            nvgFillColor(vg, nvgRGBA(0x6, 0x27, 0x37, 255));
            nvgFill(vg);
        }
        {
            float pos[4] = {30.0f*i, 0.0f, 30.0f, 150.0f};
            boarder(4, pos);
            drawVZSlider(vg, SPLAT(pos));
        }
        {
            float pos[4] = {30.0f*i, 200.0f, 30.0f, 90.0f};
            boarder(4, pos);
            drawVZSlider(vg, SPLAT(pos));
        }
    }
}


//------------------------------------------------------------------------------

void drawLogPlot(NVGcontext *vg, float *X, int x, int y, int w, int h)
{
    float dBmin = X[0];
    float dBmax = X[1];
    float fMin  = X[2];
    float fMax  = X[3];

    char dash[] = {5,5,0};
    //Db grid is easy, it is just a linear spacing
    for(int i=0; i<8; ++i) {
        int level = y+h*i/8.0;
        nvgBeginPath(vg);
        //fl_line(x, level, x()+w(), level);
    }


    //The frequency grid is defined with points at
    //10,11,12,...,18,19,20,30,40,50,60,70,80,90,100,200,400,...
    //Thus we find each scale that we cover and draw the nine lines unique to
    //that scale
    const int min_base = log(fMin)/log(10);
    const int max_base = log(fMax)/log(10);
    const float b = log(fMin)/log(10);
    const float a = log(fMax)/log(10)-b;
    for(int i=min_base; i<=max_base; ++i) {
        for(int j=1; j<10; ++j) {
            const float freq = pow(10.0, i)*j;
            const float xloc = (log(freq)/log(10)-b)/a;
            if(xloc<1.0 && xloc > -0.001)
                ;//fl_line(xloc*w()+x(), y(), xloc*w()+x(), y()+h());
        }
    }

    ;//fl_end_line();

    //Place the text labels
    char label[1024];
    for(int i=0; i<8; ++i) {
        int level = y+h*i/8.0;
        float value = (1-i/8.0)*(dBmax-dBmin) + dBmin;
        sprintf(label, "%.2f dB", value);
        ;//fl_draw(label, x+w, level);
    }

    for(int i=min_base; i<=max_base; ++i) {
        {
            const float freq = pow(10.0, i)*1;
            const float xloc = (log(freq)/log(10)-b)/a;
            sprintf(label, "%0.f Hz", freq);
            if(xloc<1.0)
                ;//fl_draw(label, xloc*w()+x()+10, y()+h());
        }
    }
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
