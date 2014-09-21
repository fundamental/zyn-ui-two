#include "nanovg/nanovg.h"

#ifdef __cplusplus
extern "C" {
#endif
void viewLFO(NVGcontext *vg, int x, int y, int h, int w);
void viewFilterEnv(NVGcontext *vg, int x, int y, int w, int h);
void viewReverb(NVGcontext *vg, int x, int y, int w, int h);
void viewAmplitude(NVGcontext *vg, int x, int y, int w, int h);
#ifdef __cplusplus
}
#endif
