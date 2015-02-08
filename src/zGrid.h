#pragma once
#include "zWidget.h"
#include "../grid.h"
#include <cassert>

class zGrid:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(int rows MEMBER m_rows);
    Q_PROPERTY(int cols MEMBER m_cols);
public:
    zGrid()
        :m_rows(1), m_cols(1)
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
        grid_t grid = gridCreate(layout_pars, m_rows, m_cols);
        for(int i=0; i<childItems().size(); ++i)
        {
            QObject *obj = childItems()[i];
            //printf("layout = '%s'\n", obj->metaObject()->className());
            if(QString("QQuickRepeater") == obj->metaObject()->className())
                continue;
        }
        gridFlow(grid, 0, 0, width(), height());
        int j=0;
        for(int i=0; i<childItems().size(); ++i)
        {
            QQuickItem *obj = dynamic_cast<QQuickItem*>(childItems()[i]);
            if(QString("QQuickRepeater") == obj->metaObject()->className())
                continue;
            float pos[4];
            gridGet(grid, j++, pos);
            printf("setpos(%f,%f,%f,%f)\n", pos[0], pos[1], pos[2], pos[3]);
            obj->setX(pos[0]);
            obj->setY(pos[1]);
            obj->setWidth(pos[2]);
            obj->setHeight(pos[3]);
        }
    }
    virtual void paint(NVGcontext *vg){};
protected:
    int m_rows;
    int m_cols;
};
