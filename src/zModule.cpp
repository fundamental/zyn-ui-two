#include "zModule.h"
#include "zImplicitLabel.h"
#include "../draw.h"

void zModule::paint(NVGcontext *vg)
{
    drawModuleBox(vg, getLabel().c_str(), 0, 0, width(), height());
}

void zModule::updateImplicit(void)
{
    printf("Implicit update...\n");
    for(int i=0; i<childItems().size(); ++i) {
        QObject *obj = childItems()[i];
        if(QString("QQuickRepeater") == obj->metaObject()->className())
            continue;
        if(QString("zImplicitLabel") == obj->metaObject()->className())
            continue;
        zWidget *w = dynamic_cast<zWidget*>(obj);
        if(!w)
            continue;
        if(!w->getLabel().empty() && !labelBoxes.contains(w)) {
            labelBoxes[w] = NULL;
            zImplicitLabel *child = new zImplicitLabel(this);
            child->m_label = w->getLabel().c_str();
            implicitChildren.push_back(child);
            labelBoxes[w] = child;
        } else if(!w->getLabel().empty() && labelBoxes[w]) {
            labelBoxes[w]->m_label = w->getLabel().c_str();
        }
    }
}
