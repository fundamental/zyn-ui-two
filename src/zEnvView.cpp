#include "zEnvView.h"

zEnvView::zEnvView()
{
    for(int i=0; i<16; ++i)
        m_data[i] = rand()*1.0/RAND_MAX;
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zEnvView::paint(NVGcontext *vg)
{
    float in[10] = {0.0, 0.0, 0.5, 0.2, 0.3, 0.7,-0.9,0.8,0.0,1.0};
    drawEnvEdit(vg, in, 5, 1, 0, 0, width(), height());
}
