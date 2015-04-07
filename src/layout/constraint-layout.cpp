#include "constraint-layout.h"

/******************************************************************
 *                 Constraint                                     *
 *****************************************************************/

void Constraint::addVar(float scale_, Variable &v_) {
    assert(v_.id>=0);
    scale.push_back(scale_);
    v.push_back(&v_);
}

bool Constraint::isConstOnly()
{
    for(int i=0;i<v.size(); ++i)
        if(v[i]->id != 0)
            return false;
    return true;
}
bool Constraint::isTrivialLowerBound()
{
    if(type != CONSTRAINT_GE || v.size() != 1)
        return false;
    if(v[0]->id == 0)
        return false;
    return true;
}

bool Constraint::isScalarEquality()
{
    return type == 0 && v.size() == 1;
}


int sign(float t) {
    return t<0?-1:(t>0?+1:0);
}
void Constraint::dump(const char *prefix)
{
    auto &cc = *this;
    if(!cc.active)
        return;
    const char *op = cc.type == 0 ? "===" : cc.type == 1 ? "<=" : ">=";
    int id = -1;
    if(constrained)
        id = constrained->id;
    printf("%s%s%%%d %s ", prefix, prefix, id, op);
    for(int j=0; j<cc.v.size(); ++j) {
        if(j!=0) {
            if(cc.scale[j] == 1) {
                printf(" + ");
                cc.v[j]->printName(true);
            } else if(cc.scale[j] == -1) {
                printf(" - ");
                cc.v[j]->printName(true);
            } else if(sign(cc.scale[j]) >= 0) {
                printf(" + %f", cc.scale[j]);
                cc.v[j]->printName(false);
            } else {
                printf(" - %f", -cc.scale[j]);
                cc.v[j]->printName(false);
            }
        } else if(cc.scale[j] == 1) {
            cc.v[j]->printName(true);
        } else if(cc.scale[j] == -1) {
            printf("-");
            cc.v[j]->printName(true);
        } else if(sign(cc.scale[j]) == 0) {
            printf("0");
        } else {
            printf("%f", cc.scale[j]);
            cc.v[j]->printName(false);
        }
    }
    if(cc.isConstOnly())
        printf(" [c]");
    if(cc.isScalarEquality())
        printf(" [s]");
    if(cc.active)
        printf(" [a]");
    printf("\n");
}


/******************************************************************
 *                   Variable                                     *
 *****************************************************************/

void Variable::reduceWithSolved(Variable *var)
{
    float replace = var->solution;
    for(int i=0; i<c.size(); ++i) {
        auto &cc = *c[i];
        for(int j=0; j<cc.v.size(); ++j) {
            if(cc.v[j] == var) {
                cc.v[j] = Const;
                cc.scale[j] *= replace;
            }
        }
        cc.dedup();
    }
}

class Constant: public Variable
{
    public:
        Constant()
        {
            id = 0;
        }
};
Variable *Const = new Constant();
/******************************************************************
 *                   Layout Problem                               *
 *****************************************************************/

void LayoutProblem::addVariable(Variable *v)
{
    v->id = var.size()+1;
    var.push_back(v);
}

void LayoutProblem::addBoxVars()
{
    //printf("Inserting Boxes...\n");
    for(unsigned i=0; i<box.size(); ++i) {
        auto &b = *box[i];
        if(b.parent) {
            auto &p = *b.parent;
            b.x.name = "x";
            b.y.name = "y";
            b.w.name = "w";
            b.h.name = "h";
            b.x.priority = 100;
            b.y.priority = 100;
            b.w.priority = 200;
            b.h.priority = 200;
            //Lower Bounds
            b.x.addConstraint(CONSTRAINT_GE, 0, Const);
            b.y.addConstraint(CONSTRAINT_GE, 0, Const);
            b.w.addConstraint(CONSTRAINT_GE, 0, Const);
            b.h.addConstraint(CONSTRAINT_GE, 0, Const);

            //Upper Bounds
            b.x.addConstraint(CONSTRAINT_LE, 1, &p.w);
            b.y.addConstraint(CONSTRAINT_LE, 1, &p.h);
            b.w.addConstraint(CONSTRAINT_LE, 1, &p.w, -1, &b.x);
            b.h.addConstraint(CONSTRAINT_LE, 1, &p.h, -1, &b.y);
        }
        addVariable(&box[i]->x);
        addVariable(&box[i]->y);
        addVariable(&box[i]->w);
        addVariable(&box[i]->h);
    }
}

int confuse_merge(int a, int b)
{
    int result = 0;
    if(a>b)
        std::swap(a,b);

    if(a == -2)
        result = 2;
    else if(b == 2)
        result = 2;
    else if(a == -1 && b != 1)
        result = -1;
    else if(a == -1 && b == 1)
        result = 2;
    else if(a == 0)
        result = b;
    else
        result = a;
    //printf("<%d,%d>->%d\n", a,b,result);
    return result;
}

void LayoutProblem::depSolve()
{
    //Find nodes which have no negative correlation with equal/higher priority
    //nodes [TODO make this efficient]
    unsigned N=var.size();
    int corr_matrix[N*N];
    memset(corr_matrix, 0, sizeof(corr_matrix));
    //Init
    for(int j=0; j<N; ++j) {
        for(int k=0; k<N; ++k) {
            if(var[j]->solved || var[k]->solved || var[j]->is_fixed || var[k]->is_fixed)
                continue;
            int neg = -1*(var[j]->hasNegCorr(var[k])&&!var[j]->solved&&!var[k]->solved);
            int pos = 1*(var[j]->hasPosCorr(var[k])&&!var[j]->solved&&!var[k]->solved);
            corr_matrix[j*N+k] = confuse_merge(neg, pos);
            corr_matrix[k*N+j] = confuse_merge(corr_matrix[j*N+k],corr_matrix[k*N+j]);
            corr_matrix[j*N+k] = confuse_merge(corr_matrix[j*N+k],corr_matrix[k*N+j]);
        }
    }

    //for(int j=0; j<N; ++j) {
    //    printf("%2d",j+1);
    //    for(int k=0; k<N; ++k) {
    //        int c = corr_matrix[j*N+k];
    //        printf(c==-1?"-":c==1?"+":(c==2||c==-2)?"?":"_");
    //    }
    //    printf("\n");
    //}

    for(int i=0; i<N; ++i) {
        for(int j=0; j<N; ++j) {
            int tmp = corr_matrix[i*N+j];
            //printf("Operating on linkage [%d,%d]\n", i+1,j+1);
            if(tmp == 1 || tmp == -2 || tmp == 2) {
                for(int k=0; k<N; ++k) {
                    if(i==k)
                        continue;
                    //printf("normal corr [%d,%d] onto [%d,%d]",i+1,k+1,j+1,k+1);
                    corr_matrix[i*N+k] =
                        confuse_merge(corr_matrix[i*N+k], corr_matrix[j*N+k]);
                }
                corr_matrix[j*N+i] = corr_matrix[i*N+j] =
                    confuse_merge(corr_matrix[i*N+j], corr_matrix[j*N+i]);
            } else if(tmp == -1) {
                for(int k=0; k<N; ++k) {
                    if(i==k)
                        continue;
                    //printf("neg corr    [%d,%d] onto [%d,%d]",i+1,k+1,j+1,k+1);
                    corr_matrix[i*N+k] =
                        confuse_merge(corr_matrix[i*N+k], -corr_matrix[j*N+k]);
                    corr_matrix[k*N+i] = corr_matrix[i*N+k] =
                        confuse_merge(corr_matrix[i*N+k], corr_matrix[k*N+i]);
                }
            }
        }
    }

    //for(int j=0; j<N; ++j) {
    //    printf("%2d",j+1);
    //    for(int k=0; k<N; ++k) {
    //        int c = corr_matrix[j*N+k];
    //        printf(c==-1?"-":c==1?"+":(c==2||c==-2)?"?":"_");
    //    }
    //    printf("\n");
    //}

    for(int i=0; i<N; ++i) {
        if(var[i]->solved || var[i]->is_fixed)
            continue;
        int prior = var[i]->priority;
        int good  = true;
        for(int j=0; j<N; ++j) {
            int c = corr_matrix[i*N+j];
            if(c==-1 || c==2 || c==-2)
                if(var[j]->priority >= prior)
                    good = false;
        }
        printf("Variable %%%d maxable: %d\n", var[i]->id, good);
        if(good)
            var[i]->maximize();
    }
}

void LayoutProblem::passSolveTrivial()
{
    for(int i=0; i<(int)var.size(); ++i)
        var[i]->trySolve();
}

void LayoutProblem::passReduceWithSolved()
{
    for(int i=0; i<(int)var.size(); ++i)
        if(var[i]->solved)
            for(int j=0; j<(int)var.size(); ++j)
                var[j]->reduceWithSolved(var[i]);
}

//Invert trivial lower bounds
//(%x >= %y) -> (%y <= %x)
void LayoutProblem::passInvertTrivialBounds()
{
    for(int i=0; i<(int)var.size(); ++i) {
        for(int j=(int)var[i]->c.size()-1; 0>=j; --j) {
            if(j<0)
                break;
            auto &cc = *var[i]->c[j];
            if(cc.isTrivialLowerBound()) {
                cc.v[0]->addConstraint(CONSTRAINT_LE,
                        1/cc.scale[0], var[i]);
                var[i]->c.erase(var[i]->c.begin()+j);
            }
        }
    }
}

void LayoutProblem::passScalarVariableFixing()
{
    for(int i=0; i<(int)var.size(); ++i) {
        if(var[i]->solved)
            continue;
        if(var[i]->hasScalarEquality()) {
            var[i]->is_fixed = true;
            for(int j=0;j<(int)var.size(); ++j)
                if(j!=i)
                    var[i]->rewrite(var[j]);
        }
    }
}

void LayoutProblem::passDisableInactive()
{
    for(int i=0; i<(int)var.size(); ++i)
        if(var[i]->solved)
            for(int j=0; j<(int)var[i]->c.size(); ++j)
                if(var[i]->c[j]->isConstOnly())
                    var[i]->c[j]->active = false;
}

void LayoutProblem::passTransferSolvedConstraints()
{
    for(int i=0; i<(int)var.size(); ++i) {
        if(!var[i]->solved)
            continue;
        int j=(int)var[i]->c.size()-1;
        while(j>=0) {
            auto &cc = *var[i]->c[j];
            //printf("SOLVED THING<%d> [%d %d %d]\n", var[i]->id, cc.isScalarEquality(),  !cc.isConstOnly(), cc.type == CONSTRAINT_EQ);
            if(cc.isScalarEquality() && !cc.isConstOnly() && cc.type == CONSTRAINT_EQ) {
                cc.v[0]->addConstraint(CONSTRAINT_EQ,
                        1/cc.scale[0], var[i]);
                var[i]->c.erase(var[i]->c.begin()+j);
            } else if(cc.v.size() == 2 && cc.v[0]->id == 0 && cc.v[1]->id != 0 &&
                    cc.type == CONSTRAINT_LE && cc.scale[1] < 0)
            {
                printf("Making a new Transfered Solution Constraint(%f)...\n",
                        var[i]->solution);
                cc.dump("OLD: ");
                cc.v[1]->addConstraint(CONSTRAINT_LE,
                        -(cc.scale[0]-var[i]->solution)/cc.scale[1], cc.v[0]);
                cc.v[1]->c[cc.v[1]->c.size()-1]->dump("NEW: ");
                var[i]->c.erase(var[i]->c.begin()+j);

            }
            j--;
        }
    }
}

void LayoutProblem::passSolveBasicAlgebra()
{
    for(int i=0; i<(int)var.size(); ++i) {
        var[i]->solveReallyBasicAlgebra();
        var[i]->removeRedundantBounds();
    }
}

void LayoutProblem::passDedup()
{
    for(int i=0; i<(int)var.size(); ++i) {
        int j=(int)var[i]->c.size()-1;
        while(j>=0) {
            auto &cc = *var[i]->c[j];
            cc.dedup();
            j--;
        }
    }
}

void LayoutProblem::passTighten()
{
    for(int i=0; i<(int)var.size(); ++i) {
        var[i]->removeRedundantBounds();
    }
}

void LayoutProblem::solve()
{
    //Solve Trivial Variables
    passSolveTrivial();

    //Propigate Solutions
    passReduceWithSolved();

    printf("-------------------------------------------------\n");
    printf("--------First Dep Solve Input--------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();

    //Propigate Solutions
    passReduceWithSolved();
    passInvertTrivialBounds();
    passTransferSolvedConstraints();


    printf("-------------------------------------------------\n");
    printf("--------Second Dep Solve Input-------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();
    dump();


    //Find any cases of scalar equality and eliminate redundant vars
    passScalarVariableFixing();
    passTransferSolvedConstraints();
    passSolveBasicAlgebra();


    printf("-------------------------------------------------\n");
    printf("---------Third Dep Solve Input-------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();

    //Propigate Solutions
    passReduceWithSolved();
    passScalarVariableFixing();
    passReduceWithSolved();
    passSolveTrivial();
    passDisableInactive();
    passTransferSolvedConstraints();
    passSolveBasicAlgebra();
    passDedup();
    passTighten();

    printf("-------------------------------------------------\n");
    printf("---------Fourth Dep Solve Input------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();
    passSolveTrivial();

    //Propigate
    passTransferSolvedConstraints();
    passSolveBasicAlgebra();
    passScalarVariableFixing();
    passReduceWithSolved();
    passSolveTrivial();
    passDisableInactive();
    passTransferSolvedConstraints();
    passSolveBasicAlgebra();
    passDedup();
    passTransferSolvedConstraints();
    passTighten();

    printf("-------------------------------------------------\n");
    printf("---------Fifth Dep Solve Input-------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();
    passReduceWithSolved();
    passTransferSolvedConstraints();
    passSolveTrivial();

    //Find Any Case upper bounded by a known value

    //Find Any Case upper bounded by a known value and a minimal low priority
    //variable

    //Give Up if there are unsolved parameters and specify why layout failed?

    //Verify That no bounding boxes overlap
    check_solution();
}

void LayoutProblem::check_solution()
{
}

void LayoutProblem::dump()
{
    printf("Layout Problem:\n");
    printf("\tBoxes:     %d\n", box.size());
    for(int i=0; i<(int)box.size(); ++i) {
        box[i]->dump("\t");
    }
    printf("\tVariables: %d\n", var.size());
    for(int i=0; i<(int)var.size(); ++i) {
        var[i]->dump("\t");
    }
}














#ifdef TESTING
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
        padw[i].priority = 100;
        padh[i].priority = 100;
    }

    for(int i=0; i<9; ++i)
    {
        const int row = i/3;
        const int col = i%3;
        auto &child = children[i];
        child.parent = &top;
        if(row == 0) {
            child.y.addConstraint(CONSTRAINT_EQ, 0, Const, 0.5, padh+i);
        } else if(row == 1){
            child.y.addConstraint(CONSTRAINT_EQ, 1, &colWidth[0], 0.5, padh+i);
        } else if(row == 2){
            child.y.addConstraint(CONSTRAINT_EQ, 1, &colWidth[0], 1, &colWidth[1], 0.5, padh+i);
        }
        //child.y.addConstraint(CONSTRAINT_GE, 1, &children[i-3].y, 1, &children[i-3].h);
        if(col == 0) {
            child.x.addConstraint(CONSTRAINT_EQ, 0, Const, 0.5, padw+i);
        } else if(col == 1){
            child.x.addConstraint(CONSTRAINT_EQ, 1, &rowHeight[0], 0.5, padw+i);
        } else if(col == 2){
            child.x.addConstraint(CONSTRAINT_EQ, 1, &rowHeight[0], 1, &rowHeight[1], 0.5, padw+i);
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
    for(int i=0; i<3; ++i) {
        colWidth[i].name = "colwidth";
        prob.addVariable(&colWidth[i]);
    }
    for(int i=0; i<3; ++i) {
        rowHeight[i].name = "rowheight";
        prob.addVariable(&rowHeight[i]);
    }
    for(int i=0; i<9; ++i) {
        padh[i].name = "padh";
        prob.addVariable(&padh[i]);
        //padh[i].solve(0.0);
    }
    for(int i=0; i<9; ++i) {
        padw[i].name = "padw";
        prob.addVariable(&padw[i]);
        //padw[i].solve(0.0);
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
#endif//TESTING
