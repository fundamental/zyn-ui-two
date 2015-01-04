#pragma once
#include <QtQuick/QQuickItem>
#include "../layout.h"

class zHALayout:public QQuickItem
{
    Q_OBJECT
public:
    zHALayout()
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
        layout_t layout = layoutCreate(LAYOUT_NONE);
        for(int i=0; i<children().size(); ++i)
        {
            QObject *obj = children()[i];
            QVariant aspect_ = obj->property("aspect");
            QVariant scale_ = obj->property("zscale");
            float aspect = 1.0;
            float scale  = 1.0;
            if(aspect_.canConvert<double>())
                aspect = aspect_.toDouble();
            if(scale_.canConvert<double>() && scale_.toDouble() != 0.0)
                scale = scale_.toDouble();
              
            layoutBoundBox(layout, aspect, scale);
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

};
