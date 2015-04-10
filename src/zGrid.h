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
    {}
public slots:
    void damageLayout()
    {
        doLayout();
    }
    void doLayout()
    {
        printf("\n\n\nLAYYYYYYYYYYYYYYYYOUUUUUUUUUUUUUUUUUUUUUUUTTTTTT\n");
        printf("<0,0,%f,%f>#%d\n", width(), height(),childItems().size());

        LayoutProblem prob;
        BBox self;
        self.x = 0;
        self.y = 0;
        self.w = width();
        self.h = height();
        Variable *padh = new Variable[m_rows*m_cols];
        Variable *padw = new Variable[m_rows*m_cols];
        Variable *rh   = new Variable[m_rows];
        Variable *cw   = new Variable[m_cols];
        BBox    **ch   = new BBox*[m_rows*m_cols];

        prob.addBox(self);
        memset(ch, 0, sizeof(void*)*m_rows*m_cols);
        for(int i=0; i<childItems().size(); ++i)
        {
            QQuickItem *obj = dynamic_cast<QQuickItem*>(childItems()[i]);
            if(QString("QQuickRepeater") == obj->metaObject()->className())
                continue;
            zWidget *obj_ = dynamic_cast<zWidget*>(obj);
            if(obj_)
                ch[i] = &obj_->layoutSubProblems(prob, self);
            else
                ch[i] = new BBox;
        }


        for(int i=0; i<m_rows; ++i) {
            rh[i].name = "row-height";
            rh[i].priority = 900;
            rh[i] = height()/m_rows;
            prob.addVariable(rh+i);
        }
        for(int i=0; i<m_cols; ++i) {
            cw[i].name = "column-width";
            cw[i].priority = 50;
            cw[i] = width()/m_cols;
            prob.addVariable(cw+i);
            auto &cc = cw[i].addConstraint(CONSTRAINT_LE, 1, &self.w);
            //for(int j=0; j<m_cols; ++j)
            //    if(i!=j)
            //        cc.addVar(-1.0, cw[j]);
        }
    
        for(int i=0; i<m_rows*m_cols; ++i)
        {
            const int row = i/m_cols;
            const int col = i%m_cols;
            if(!ch[i])
                ch[i] = new BBox;
            auto &child = *ch[i];
            child.parent = &self;
            printf("row=%d,col=%d\n", row, col);

            cw[col] >= child.w;
            auto *cy = &child.y.addConstraint(CONSTRAINT_EQ, 0.5, padh+i);
            auto *cx = &child.x.addConstraint(CONSTRAINT_EQ, 0.5, padw+i);
            for(int i=0; i<row; ++i)
                cy->addVar(1.0, rh[i]);
            for(int i=0; i<col; ++i)
                cx->addVar(1.0, cw[i]);

            cy = &child.y.addConstraint(CONSTRAINT_LE, -0.5, padh+i, -1, &child.h);
            cx = &child.x.addConstraint(CONSTRAINT_LE, -0.5, padw+i, -1, &child.w);
            for(int i=0; i<row+1; ++i)
                cy->addVar(1.0, rh[i]);
            for(int i=0; i<col+1; ++i)
                cx->addVar(1.0, cw[i]);
            //auto &ch_ = child.h.addConstraint(CONSTRAINT_LE, -1, &child.y, -1, padh+i);
            //for(int i=0; i<row+1; ++i)
            //    ch_.addVar(1.0, rh[i]);
            //for(int i=0; i<col; ++i)
            //    cw_.addVar(1.0, cw[i]);
            child.w <= cw[col]-padw[i];
            child.h <= rh[row]-padh[i];
            padh[i].name = "padh";
            padw[i].name = "padw";
            padh[i].priority = 120;
            padw[i].priority = 120;
            prob.addVariable(padh+i);
            prob.addVariable(padw+i);
            prob.addBox(child);
        }
        

        prob.addBoxVars();
        prob.dump();

        prob.solve();

        int j=0;
        for(int i=0; i<childItems().size(); ++i)
        {
            QQuickItem *obj = dynamic_cast<QQuickItem*>(childItems()[i]);
            if(QString("QQuickRepeater") == obj->metaObject()->className())
                continue;
            printf("set bounds <%f, %f, %f, %f>\n",
                    ch[i]->x.solution,
                    ch[i]->y.solution,
                    ch[i]->w.solution,
                    ch[i]->h.solution);
            setBounds(*obj, *ch[i]);
        }
    }
    virtual void paint(NVGcontext *vg){};
protected:
    int m_rows;
    int m_cols;
};
