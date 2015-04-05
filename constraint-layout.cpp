#include "constraint-layout.h"

int main()
{
    BBox top;
    top.parent = 0;
    top.x.addConstraint(CONSTRAINT_EQ, 0, Const);
    top.y.addConstraint(CONSTRAINT_EQ, 0, Const);
    top.w.addConstraint(CONSTRAINT_EQ, 800, Const);
    top.h.addConstraint(CONSTRAINT_EQ, 600, Const);

    BBox children[9];
    Variable colWidth[3];
    Variable rowHeight[3];
    Variable padw[9];
    Variable padh[9];
    //init column params
    for(int i=0; i<3; ++i)
    {
        colWidth[i].priority = 900;
        for(int j=0; j<3; ++j)
            children[3*i+j].w.addConstraint(CONSTRAINT_LE, 1, colWidth+j, -1, &padw[3*i+j]);
        
        rowHeight[i].priority = 900;
        for(int j=0; j<3; ++j)
            children[i+3*j].h.addConstraint(CONSTRAINT_LE, 1, rowHeight+j, -1, &padh[i+3*j]);
    }
    //init pad params
    for(int i=0; i<9; ++i)
    {
        padw[i].priority = 0;
        padh[i].priority = 0;
    }

    for(int i=0; i<9; ++i)
    {
        const int row = i/3;
        const int col = i%3;
        auto &child = children[i];
        child.parent = &top;
        if(row == 0) {
            child.y.addConstraint(CONSTRAINT_EQ, 0, Const, 1, padh+i);
        } else if(row == 1){
            child.y.addConstraint(CONSTRAINT_EQ, 1, &colWidth[0], 1, padh+i);
        } else if(row == 2){
            child.y.addConstraint(CONSTRAINT_EQ, 1, &colWidth[0], 1, &colWidth[1], 1, padh+i);
        }
        //child.y.addConstraint(CONSTRAINT_GE, 1, &children[i-3].y, 1, &children[i-3].h);
        if(col == 0) {
            child.x.addConstraint(CONSTRAINT_EQ, 0, Const, 1, padw+i);
        } else if(col == 1){
            child.x.addConstraint(CONSTRAINT_EQ, 1, &rowHeight[0], 1, padw+i);
        } else if(col == 2){
            child.x.addConstraint(CONSTRAINT_EQ, 1, &rowHeight[0], 1, &rowHeight[1], 1, padw+i);
        } else {
        }
        //child.x.addConstraint(CONSTRAINT_GE, 1, &children[i-1].x, 1, &children[i-1].w);
    }


    children[4].w.addConstraint(CONSTRAINT_EQ, 1, &children[4].h);

    colWidth[0].addConstraint(CONSTRAINT_EQ, 1, &colWidth[1]);
    colWidth[1].addConstraint(CONSTRAINT_EQ, 1, &colWidth[2]);
    rowHeight[0].addConstraint(CONSTRAINT_EQ, 1, &rowHeight[1]);
    rowHeight[1].addConstraint(CONSTRAINT_EQ, 1, &rowHeight[2]);
    top.w.addConstraint(CONSTRAINT_EQ, 1, &colWidth[0], 1, &colWidth[1], 1, &colWidth[2]);
    top.h.addConstraint(CONSTRAINT_EQ, 1, &rowHeight[0], 1, &rowHeight[1], 1, &rowHeight[2]);
    
    LayoutProblem prob;

    // BBox A;
    // BBox B;
    // A.parent = &top;
    // B.parent = &top;
    // A.x.addConstraint(CONSTRAINT_EQ, 0,   Const);
    // B.x.addConstraint(CONSTRAINT_EQ, 1,   &subproblem1.x, 1, &subproblem1.w);
    // //subproblem2.w.addConstraint(CONSTRAINT_EQ, 0.3333, &subproblem1.w);

    //Variable tmp;
    //tmp.priority = 500;
    for(int i=0; i<9; ++i)
        prob.addBox(children[i]);
    prob.addBox(top);
    for(int i=0; i<3; ++i)
        prob.addVariable(&colWidth[i]);
    for(int i=0; i<3; ++i)
        prob.addVariable(&rowHeight[i]);
    for(int i=0; i<9; ++i) {
        prob.addVariable(&padh[i]);
        padh[i].solve(0.0);
    }
    for(int i=0; i<9; ++i) {
        prob.addVariable(&padw[i]);
        padw[i].solve(0.0);
    }

    prob.addBoxVars();
    prob.dump();
    //prob.addVariable(&tmp);
    //A.w.addConstraint(CONSTRAINT_EQ, 2, &tmp);
    //B.w.addConstraint(CONSTRAINT_EQ, 6, &tmp);
    //prob.addBox(top);
    //prob.addBox(subproblem1);
    //prob.addBox(subproblem2);

    prob.solve();
    prob.dump();


    //Verify That no widgets with the same parent overlap
    prob.check_solution();
}
