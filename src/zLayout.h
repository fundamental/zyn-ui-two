#pragma once
#include "zWidget.h"
#include "zImplicitLabel.h"
#include "../layout.h"
#include <QMap>
#include <cassert>

class zLayout:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(bool vertical MEMBER m_vertical);
public:
    zLayout()
        :m_vertical(false)
    {
        connect(this, SIGNAL(xChanged()), this, SLOT(doLayout()));
        connect(this, SIGNAL(yChanged()), this, SLOT(doLayout()));
        connect(this, SIGNAL(heightChanged()), this, SLOT(doLayout()));
        connect(this, SIGNAL(widthChanged()), this, SLOT(doLayout()));
        connect(this, SIGNAL(childrenChanged()), this, SLOT(doLayout()));
    }
public slots:
    void doLayout()
    {
        int layout_pars  = LAYOUT_NONE;
        if(m_vertical)
            layout_pars |= LAYOUT_VERT;
        if(!implicitChildren.empty())
            layout_pars |= LAYOUT_LABELS;
        layout_t layout = layoutCreate(layout_pars);
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
              
            layoutBoundBox(layout, aspect, scale);
            if(expandable) {
                layoutExpandable(layout);
            }
        }
        layoutFlow(layout, 0, layoutY(), width(), layoutH());
        int j=0;
        for(int i=0; i<childItems().size(); ++i)
        {
            QQuickItem *obj = dynamic_cast<QQuickItem*>(childItems()[i]);
            if(QString("QQuickRepeater") == obj->metaObject()->className())
                continue;
            if(QString("zImplicitLabel") == obj->metaObject()->className())
                continue;
            float pos[4];
            layoutGet(layout, j++, pos);
            obj->setX(pos[0]);
            obj->setY(pos[1]);
            obj->setWidth(pos[2]);
            obj->setHeight(pos[3]);
            if(labelBoxes.contains(obj) && labelBoxes[obj]) {
                obj = labelBoxes[obj];
                layoutGetLabel(layout, j-1, pos);
                obj->setX(pos[0]);
                obj->setY(pos[1]);
                obj->setWidth(pos[2]);
                obj->setHeight(pos[3]);
            }
        }
    }
    virtual void paint(NVGcontext *vg){};
protected:
    virtual float layoutY() const {return 0.0f;};
    virtual float layoutH() const {return height();};
    QList<QQuickItem*> implicitChildren;
    QMap<QQuickItem *, class zImplicitLabel *> labelBoxes;
    bool m_vertical;
};
