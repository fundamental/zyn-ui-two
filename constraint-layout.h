#include <vector>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <string>

#define CONSTRAINT_EQ 0
#define CONSTRAINT_LE 1
#define CONSTRAINT_GE 2

class Variable;
class Constraint
{
    public:
        Variable                *constrained;
        std::vector<Variable *>  v;
        std::vector<float>       scale;
        int                      type;//0 equality, 1 upper bounded, 2 lower bounded
        bool                     active;
        bool                     satisfied;
        int                      id;
        unsigned size() const {return v.size();}
        bool       canSimplify(Constraint &c);
        Constraint simplify(Constraint &c);
        bool isTrivialLowerBound();
        bool isScalarEquality();
        void dedup()
        {
            unsigned N=v.size();
            //Eliminate Zeros
            int current_id=N-1;
            while(current_id >= 0) {
                if(fabs(scale[current_id]) < 1e-6 && (current_id != 0 || v.size()>1)) {
                    v.erase(v.begin()+current_id);
                    scale.erase(scale.begin()+current_id);
                }
                current_id--;
            }
            //Eliminate Duplicate Entries
            N=v.size();
            current_id=N-1;
            while(current_id > 0) {
                int sub = current_id - 1;
                while(sub >= 0)
                {
                    if(v[sub] == v[current_id]) {
                        scale[sub] += scale[current_id];
                        v.erase(v.begin()+current_id);
                        scale.erase(scale.begin()+current_id);
                    }
                    sub--;
                }
                current_id--;
            }
        }

        void dump(const char *prefix);

        bool isConstOnly();
};

class Variable
{
    public:
        Variable()
            :solved(false), priority(1), is_fixed(false), id(10101) {}
        Variable(const Variable&) = delete;
        float fixed_value;
        bool  is_fixed;

        float solution;
        bool  solved;

        int priority; //0 low, 1 normal
        int id;
        std::string name;

        Variable *alias;//Pointer to variable that actually solves problem

        void solve(float s)
        {
            //assert(id != 41);
            solution = s;
            solved   = true;
            for(int i=0; i<c.size(); ++i)
                if(c[i]->isConstOnly())
                    c[i]->active = false;
        }

        void addConstraint(int type, float scale, Variable *v)
        {
            c.push_back(new Constraint{this, {v}, {scale}, type, 1,0,(int)c.size()});
        }
        void addConstraint(int type, float scale, Variable *v, float scale2, Variable *v2)
        {
            c.push_back(new Constraint{this, {v,v2}, {scale,scale2}, type, 1,0,(int)c.size()});
        }
        void addConstraint(int type,
                float s1, Variable *v1,
                float s2, Variable *v2,
                float s3, Variable *v3)
        {
            c.push_back(new Constraint{this, {v1,v2,v3}, {s1,s2,s3}, type, 1,0,(int)c.size()});
        }

        void solveReallyBasicAlgebra()
        {
            //If any constraint consists of the form
            //%x OP %a + %b%x
            //then reduce it to
            //%x OP %a/(1-b)
            for(int i=0; i<(int)c.size(); ++i)
            {
                auto &cc = *c[i];
                int self = -1;
                for(int j=0; j<(int)cc.v.size(); ++j)
                    if(cc.v[j] == this)
                        self = j;

                if(self == -1)
                    continue;
                float rescale = 1/(1-cc.scale[self]);
                cc.v.erase(cc.v.begin()+self);
                cc.scale.erase(cc.scale.begin()+self);
                for(int j=0; j<(int)cc.scale.size(); ++j)
                    cc.scale[j] *= rescale;
                cc.dedup();
            }
        }

        //True if b is redundant if a exists
        bool redundantBound(Constraint &a, Constraint &b)
        {
            if(a.size() != b.size() || a.type != b.type)
                return false;
            if(a.size() == 1 && a.v[0] == b.v[0] && a.scale[0] == b.scale[0])
                return true;
            if(a.type == CONSTRAINT_LE) {
                bool all = true;
                for(int i=0; i<a.size(); ++i)
                    if(a.v[i] != b.v[i] || a.scale[i] > b.scale[i])
                        all = false;
                if(all)
                    return true;
            }
            return false;
        }

        void removeRedundantBounds()
        {
            bool to_remove[c.size()];
            memset(to_remove, 0, sizeof(to_remove));
            for(int i=0; i<(int)c.size(); ++i) {
                for(int j=0; j<(int)c.size(); ++j) {
                    if((to_remove[i] || to_remove[j]) || i==j)
                        continue;
                    to_remove[j] = redundantBound(*c[i],*c[j]);
                }
            }

            for(int i=c.size()-1; i>=0; --i)
                if(to_remove[i])
                    c.erase(c.begin()+i);
        }

        void trySolve()
        {
            if(solved)
                return;
            //printf("TRY TO SOLVE MEEEEEEEEEEEEEEEEEEEEEE<%d>\n", id);
            for(int i=0; i<(int)c.size(); ++i) {
                auto &cc = *c[i];
                //printf("type = %d\n", cc.type);
                //printf("size = %d\n", cc.v.size());
                //printf("id   = %d\n", cc.v[0]->id);
                if(cc.type == 0 && cc.v.size() == 1 && cc.v[0]->id == 0)
                    solve(cc.scale[i]);
                else if(cc.type == 0 && cc.v.size() == 1 && cc.v[0]->solved)
                    solve(cc.scale[0]*cc.v[0]->solution);
            }
        }

        void maximize()
        {
            //In case where there is only one usable max rule
            //consisting of a constant and lower priority variables
            Constraint *rule = NULL;
            Constraint *unhandled_eq = NULL;
            for(int i=0; i<(int)c.size(); ++i) {
                if(c[i]->type == 1) {
                    if(!rule)
                        rule = c[i];
                    else {
                        printf("Weird maximization criteria\n");
                        //rule->dump("OLD:");
                        //c[i]->dump("NEW:");
                        if(rule->scale[0] > c[i]->scale[0])
                            rule = c[i];
                    }
                } else if(c[i]->type == CONSTRAINT_EQ) {
                    unhandled_eq = c[i];
                    unhandled_eq->dump("Possible issue:");
                }
            }

            if(!rule || unhandled_eq)
                return;
            for(int i=0; i<rule->v.size(); ++i)
                if(rule->v[i]->id == 0)
                    solve(rule->scale[i]);
        }

        void reduceWithSolved(Variable *var);

        bool hasScalarEquality()
        {
            Constraint *rule = NULL;
            for(int i=0; i<(int)c.size(); ++i)
                if(c[i]->type == 0 && c[i]->v.size() == 1)
                    rule = c[i];
            if(!rule)
                return false;
            return true;
        }

        void inverseRewrite(float scale, Variable *var)
        {
            //printf("inverse rewrite()\n");
            for(int i=0; i<var->c.size(); ++i) {
                auto &cc = *var->c[i];
                if(cc.type != 0) {
                    auto *ncon = new Constraint();
                    ncon->type = cc.type;
                    for(int j=0; j<cc.v.size(); ++j) {
                        //printf("tick...\n");
                        ncon->v.push_back(cc.v[j]);
                        ncon->scale.push_back(scale*cc.scale[j]);
                    }
                    c.push_back(ncon);
                    ncon->dedup();
                }
                cc.dedup();
            }
        }

        void rewrite(Variable *var)
        {
            //printf("Rewrite(using %%%d on %%%d)...\n",id,var->id);
            Constraint *rule = NULL;
            for(int i=0; i<(int)c.size(); ++i)
                if(c[i]->type == 0 && c[i]->v.size() == 1)
                    rule = c[i];
            if(!rule)
                return;
            //else
            //    printf("Found Rule...\n");
            Variable *rep    = rule->v[0];
            float rep_scale =  rule->scale[0];
            //printf("rewrier = %%%d %p %p\n", rep->id, var, rep);
            if(rep == var)
                var->inverseRewrite(1/rep_scale, this);
            else {
                for(int i=0; i<var->c.size(); ++i) {
                    auto &cc = *var->c[i];
                    for(int j=0; j<cc.v.size(); ++j) {
                        if(cc.v[j] == this) {
                            cc.v[j]      = rep;
                            cc.scale[j] *= rep_scale;
                        }
                    }
                    cc.dedup();
                }
            }
        }

        void printName(bool force)
        {
            assert(id >= 0);
                
            if(id==0 && force) {
                printf("1");
            } else if(id != 0) {
                printf("%%%d", id);
            }
        }

        int sign(float t) {
            return t<0?-1:(t>0?+1:0);
        }

        void dump(const char *prefix)
        {
            return;
            printf("%sVariable %d<%s>: [", prefix, id, name.c_str());
            if(solved)
                printf("solved<%f>,", solution);
            if(is_fixed)
                printf("fixed_value,");
            printf("priority<%d>]\n", priority);

            for(int i=0; i<(int)c.size(); ++i) {
                c[i]->dump(prefix);
            }
        }

        float hasNegCorr(Variable *var)
        {
            for(auto cc:c)
                for(int i=0; i<cc->v.size(); ++i)
                    if(var == cc->v[i] && sign(cc->scale[i]) == -1)
                        return true;
            return false;
        }
        float hasPosCorr(Variable *var)
        {
            for(auto cc:c)
                for(int i=0; i<cc->v.size(); ++i)
                    if(var == cc->v[i] && sign(cc->scale[i]) == +1)
                        return true;
            return false;
        }

        std::vector<Constraint *> c;
};
        

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

class BBox
{
    public:
        int id=0;
        BBox *parent;
        Variable x,y,w,h;

        void dumpVar(Variable &var)
        {
            if(var.solved)
                printf("%f",var.solution);
            else
                printf("?");
        }
        void dump(const char *prefix)
        {
            printf("%s%s%d - <", prefix,prefix,id);
            dumpVar(x);printf(",");
            dumpVar(y);printf(",");
            dumpVar(w);printf(",");
            dumpVar(h);printf(">\n");
        }
};

class LayoutProblem
{
    public:
        std::vector<BBox *>     box;
        std::vector<Variable *> var;
        void addVariable(Variable *var);
        void addBoxVars();
        void solve();
        void depSolve();
        void passSolveTrivial();
        void passReduceWithSolved();
        void passInvertTrivialBounds();
        void passScalarVariableFixing();
        void passDisableInactive();
        void passRemoveRedundant();
        void passTransferSolvedConstraints();
        void passSolveBasicAlgebra();
        void passDedup();
        void passTighten();

        void check_solution();
        void dump();
        void addBox(BBox &b)
        {
            b.id = box.size();
            box.push_back(&b);
        }
};

void LayoutProblem::addVariable(Variable *v)
{
    v->id = var.size()+1;
    var.push_back(v);
}

class Constant: public Variable
{
    public:
        Constant()
        {
            id = 0;
        }
};

Constant *Const = new Constant();

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
