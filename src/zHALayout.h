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
        printf("Doing layout(%d)...\n", children().size());
        layout_t layout = layoutCreate(LAYOUT_NONE);
        for(int i=0; i<children().size(); ++i)
        {
            QObject *obj = children()[i];
            QVariant aspect_ = obj->property("aspect");
            QVariant scale_ = obj->property("scale");
            float aspect = 1.0;
            float scale  = 1.0;
            if(aspect_.canConvert<double>())
                aspect = aspect_.toDouble();
            if(scale_.canConvert<double>() && aspect_.toDouble() != 0.0)
                scale = aspect_.toDouble();
              
            printf("layoutPar %d: %f %f\n", i, aspect, scale);
            layoutBoundBox(layout, aspect, scale);
        }
        layoutFlow(layout, 0, 0, width(), height());
        for(int i=0; i<children().size(); ++i)
        {
            QQuickItem *obj = dynamic_cast<QQuickItem*>(children()[i]);
            float pos[4];
            layoutGet(layout, i, pos);
            printf("layout %d: %f %f %f %f\n", i, pos[0], pos[1], pos[2], pos[3]);
            obj->setX(pos[0]);
            obj->setY(pos[1]);
            obj->setWidth(pos[2]);
            obj->setHeight(pos[3]);
        }
    }

};
