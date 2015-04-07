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
        self.x.addConstraint(CONSTRAINT_EQ, 0, Const);
        self.y.addConstraint(CONSTRAINT_EQ, 0, Const);
        self.w.addConstraint(CONSTRAINT_EQ, width(), Const);
        self.h.addConstraint(CONSTRAINT_EQ, height(), Const);
        Variable *padh = new Variable[m_rows*m_cols];
        Variable *padw = new Variable[m_rows*m_cols];
        Variable *rh   = new Variable[m_rows];
        Variable *cw   = new Variable[m_cols];
        BBox     *ch   = new BBox[m_rows*m_cols];

        prob.addBox(self);
        for(int i=0; i<m_rows; ++i) {
            rh[i].name = "row-height";
            rh[i].priority = 900;
            rh[i].addConstraint(CONSTRAINT_EQ, height()/m_rows, Const);
            prob.addVariable(rh+i);
        }
        for(int i=0; i<m_cols; ++i) {
            cw[i].name = "column-width";
            cw[i].priority = 900;
            cw[i].addConstraint(CONSTRAINT_EQ, width()/m_rows, Const);
            prob.addVariable(cw+i);
        }
    
        for(int i=0; i<m_rows*m_cols; ++i)
        {
            const int row = i/3;
            const int col = i%3;
            auto &child = ch[i];
            child.parent = &self;
            auto &cy = child.y.addConstraint(CONSTRAINT_EQ, 0.5, padh+i);
            auto &cx = child.x.addConstraint(CONSTRAINT_EQ, 0.5, padw+i);
            for(int i=0; i<row; ++i)
                cy.addVar(1.0, cw[i]);
            for(int i=0; i<col; ++i)
                cx.addVar(1.0, rh[i]);
            child.w.addConstraint(CONSTRAINT_LE, 1, &cw[col]);
            child.h.addConstraint(CONSTRAINT_LE, 1, &rh[row]);
            padh[i].name = "padh";
            padw[i].name = "padw";
            padh[i].priority = 100;
            padw[i].priority = 100;
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
            setBounds(*obj, ch[j]);
        }
    }
    virtual void paint(NVGcontext *vg){};
protected:
    int m_rows;
    int m_cols;
};
