#pragma once
#include "zWidget.h"
#include "../layout.h"
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
signals:

public slots:
    void doLayout()
    {
        int layout_pars  = LAYOUT_NONE;
        if(m_vertical)
            layout_pars |= LAYOUT_VERT;
        layout_t layout = layoutCreate(layout_pars);
        for(int i=0; i<children().size(); ++i)
        {
            QObject *obj = children()[i];
            QVariant aspect_ = obj->property("aspect");
            QVariant scale_ = obj->property("zscale");
            QVariant expandable_ = obj->property("expandable");
            float aspect = 1.0;
            float scale  = 1.0;
            bool  expandable = false;
            if(aspect_.canConvert<double>())
                aspect = aspect_.toDouble();
            if(scale_.canConvert<double>() && scale_.toDouble() != 0.0)
                scale = scale_.toDouble();
            if(expandable_.canConvert<bool>())
                expandable = expandable_.toBool();
              
            layoutBoundBox(layout, aspect, scale);
            if(expandable) {
                layoutExpandable(layout);
            }
        }
        layoutFlow(layout, 0, 0, width(), height());
        for(int i=0; i<children().size(); ++i)
        {
            QQuickItem *obj = dynamic_cast<QQuickItem*>(children()[i]);
            float pos[4];
            layoutGet(layout, i, pos);
            obj->setX(pos[0]);
            obj->setY(pos[1]);
            obj->setWidth(pos[2]);
            obj->setHeight(pos[3]);
        }
    }
    virtual void paint(NVGcontext *vg){};
protected:
    bool m_vertical;
};
