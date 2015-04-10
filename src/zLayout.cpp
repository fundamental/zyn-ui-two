#include "zLayout.h"

int zLayout::getLayoutChildren()
{
    int j=0;
    for(int i=0; i<childItems().size(); ++i)
    {
        QQuickItem *obj = dynamic_cast<QQuickItem*>(childItems()[i]);
        if(QString("QQuickRepeater") == obj->metaObject()->className())
            continue;
        if(QString("zImplicitLabel") == obj->metaObject()->className())
            continue;
        j++;
    }
    return j;
}

void zLayout::doLayout()
{
    //Assume m_vertical = false for now
    LayoutProblem prob;
    BBox self;
    self.x = 0;
    self.y = layoutY();
    self.w = width();
    self.h = layoutH();
    Variable *rh = new Variable();
    Variable *sv = new Variable();
    Variable *ph = new Variable[getLayoutChildren()];
    BBox    **ch = new BBox*[getLayoutChildren()];
    if(layoutH() == 0 || width() == 0)
        return;

    rh->name = "row-height";
    sv->name = "scale-variable";

    prob.addBox(self);
    prob.addVariable(sv);
    prob.addVariable(rh);

    int j=0;
    for(int i=0; i<childItems().size(); ++i)
    {
        QObject *obj = childItems()[i];
        //printf("layout = '%s'\n", obj->metaObject()->className());
        if(QString("QQuickRepeater") == obj->metaObject()->className())
            continue;
        if(QString("zImplicitLabel") == obj->metaObject()->className())
            continue;
        QVariant aspect_ = obj->property("aspect");
        QVariant scale_ = obj->property("zscale");
        QVariant expandable_ = obj->property("expandable");
        float aspect = 1.0;
        float scale  = 1.0;
        bool  expandable = false;
        if(aspect_.canConvert<double>())
            aspect = aspect_.toDouble();
        if(scale_.canConvert<double>())
            scale = scale_.toDouble();
        if(expandable_.canConvert<bool>())
            expandable = expandable_.toBool();

        zWidget *obj_ = dynamic_cast<zWidget*>(obj);
        if(obj_)
            ch[j] = &obj_->layoutSubProblems(prob, self);
        else
            ch[j] = new BBox;
        *sv = scale*ch[j]->h;
        if(j == 0) {
            ch[j]->x = 0;
        } else {
            ch[j]->x = ch[j-1]->x + ch[j-1]->w;
        }
        ch[j]->parent = &self;
        ch[j]->y = 0.5*ph[j];
        ch[j]->h <= self.h-ph[j];
        ph[j].name = "pad-height";
        ph[j].priority = 120;
        prob.addBox(*ch[j]);
        prob.addVariable(ph+j);
        ph[j] = 0;


        //layoutBoundBox(layout, aspect, scale);
        //if(expandable) {
        //    layoutExpandable(layout);
        //}
        j++;
    }
    prob.addBoxVars();
    prob.dump();
    prob.solve();

    j=0;
    for(int i=0; i<childItems().size(); ++i)
    {
        QQuickItem *obj = dynamic_cast<QQuickItem*>(childItems()[i]);
        if(QString("QQuickRepeater") == obj->metaObject()->className())
            continue;
        if(QString("zImplicitLabel") == obj->metaObject()->className())
            continue;
        BBox *box = ch[j];
        setBounds(*obj, *box);
        //if(labelBoxes.contains(obj) && labelBoxes[obj]) {
        //    obj = labelBoxes[obj];
        //    layoutGetLabel(layout, j-1, pos);
        //    obj->setX(pos[0]);
        //    obj->setY(pos[1]);
        //    obj->setWidth(pos[2]);
        //    obj->setHeight(pos[3]);
        //}
        j++;
    }
}
