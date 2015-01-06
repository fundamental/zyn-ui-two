#include "zTextList.h"
#include "../nanovg/nanovg.h"

zTextList::zTextList()
{
    strings.push_back("Organs");
    strings.push_back("Synth Lead");
    strings.push_back("Synth Pad");
    strings.push_back("Synth Misc");
    strings.push_back("Guitar");
    strings.push_back("Plucked String");
    strings.push_back("Bass");
    strings.push_back("Drums");
    strings.push_back("Percussion");
    strings.push_back("Mallet Instruments");
    strings.push_back("Flute");
    strings.push_back("Reed Instruments");
    strings.push_back("Brass");
    strings.push_back("Bowed Strings");
    strings.push_back("Vocal");
    strings.push_back("Soundscapes");
    strings.push_back("Sound Effects");
    strings.push_back("Mulittrack");
}

NVGcontext *initVG();
//Turns out that this overcomplicates things...
static float badness(float height, float width, QStringList list)
{
    NVGcontext *vg = initVG();
    float bounds[4];
	nvgFontSize(vg, height*0.9);
	nvgFontFace(vg, "sans");

    float badness = 0;
    for(int i=0; i<list.size(); ++i) {
        nvgTextBounds(vg, 0, 0, list[i].toLatin1().data(), NULL, bounds);
        float newWidth = bounds[2]-bounds[0];
        if(newWidth > width) {
            //printf("OVERFLOW\n");
            return 10000;
        } else {
            badness += 0.1*(width-newWidth);
        }
    }
    return badness;
}

static float flowSingle(QRectF bb, QStringList list)
{
    float minBadness = 1e99;
    float indBest = -1;

    int i = 30;
    float bad = badness(bb.height()/i, bb.width(), list);
    minBadness = bad;
    indBest = i;

    float best = bb.height()/indBest;
    //for(float i=-1; i<1; i+=0.01) {
    //    float bad = badness(bb.height()/(indBest+i), bb.width(), list);
    //    if(bad < minBadness) {
    //        minBadness = bad;
    //        best = bb.height()/(indBest+i);
    //    }
    //}

    if(best < 18)
        return 18;
    return best;
}

void zTextList::paint(NVGcontext *vg)
{
    float boxHeight = flowSingle(boundingRect(), strings);

    for(int i=0; i<strings.size(); ++i) {
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(0x06, 0x22, 0x37, 255));
        nvgRect(vg, 0,i*boxHeight+0.025*boxHeight,width(),0.95*boxHeight);
        nvgFill(vg);

        nvgFontSize(vg, boxHeight*0.9);
        nvgFontFace(vg, "sans");
        nvgFillColor(vg, nvgRGBA(0x3a, 0xc5, 0xec, 255));
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgText(vg, 0, i*boxHeight+boxHeight*0.5f, strings[i].toLatin1().data(), NULL);
    }
}
