#include "constraint-layout.h"

#define QUIET_DOWN
#ifdef  QUIET_DOWN
#define printf(...)
#endif

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
        
bool Constraint::isStrictlyNegativeUpper()
{
    if(type != CONSTRAINT_LE)
        return false;
    for(int i=0;i<scale.size();++i)
        if(scale[i]>0)
            return false;
    return true;
}

bool Constraint::isConstAndNeg()
{
    if(type != CONSTRAINT_LE || scale.size() < 2)
        return false;
    if(v[0]->id != 0)
        return false;
    for(int i=1;i<scale.size();++i)
        if(scale[i]>0)
            return false;
    return true;
}
        
void Constraint::dedup()
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
    //Put constant term in front
    N=v.size();
    if(v.size() != 0 && v[0]->id != 0) {
        for(int i=0; i<N; ++i) {
            if(v[i]->id == 0) {
                std::swap(v[i], v[0]);
                std::swap(scale[i], scale[0]);
                break;
            }
        }
    }
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
 *                   Linear Constraint                            *
 *****************************************************************/
LinearConstraint::LinearConstraint(Variable&v_)
{
    v.push_back(&v_);
}
LinearConstraint::LinearConstraint(float s_)
{
    s.push_back(s_);
}

void LinearConstraint::finalize()
{
    if(s.size() == v.size() + 1)
        v.push_back(Const);
    if(s.size() == v.size() - 1)
        s.push_back(1.0);
    assert(s.size() == v.size());
}

LinearConstraint operator-(LinearConstraint self, LinearConstraint l)
{
    self.finalize();
    l.finalize();
    assert(self.s.size() == self.v.size());
    assert(l.s.size() == l.v.size());
    for(int i=0; i<l.s.size(); ++i) {
        self.s.push_back(-l.s[i]);
        self.v.push_back(l.v[i]);
    }
    return self;
}

LinearConstraint operator+(LinearConstraint self, LinearConstraint l)
{
    self.finalize();
    l.finalize();
    assert(self.s.size() == self.v.size());
    assert(l.s.size() == l.v.size());
    for(int i=0; i<l.s.size(); ++i) {
        self.s.push_back(l.s[i]);
        self.v.push_back(l.v[i]);
    }
    return self;
}

LinearConstraint operator*(LinearConstraint self, LinearConstraint l)
{
    assert(self.s.size() == 1);
    assert(self.v.size() == 0);
    assert(l.s.size() == 0);
    assert(l.v.size() == 1);
    self.v.push_back(l.v[0]);
    return self;
}

LinearConstraint operator/(LinearConstraint self, LinearConstraint l)
{
    assert(self.v.size() == 1);
    assert(self.s.size() == 0);
    assert(l.v.size() == 0);
    assert(l.s.size() == 1);
    self.s.push_back(1.0/l.s[0]);
    return self;
}


/******************************************************************
 *                   Variable                                     *
 *****************************************************************/

void Variable::solve(float s)
{
    //assert(id != 41);
    solution = s;
    solved   = true;
    for(int i=0; i<c.size(); ++i)
        if(c[i]->isConstOnly())
            c[i]->active = false;
}

Constraint &Variable::addConstraint(int type, float scale, Variable *v)
{
    assert(v->id>=0);
    auto *cc = new Constraint{this, {v}, {scale}, type, 1,0,(int)c.size()};
    c.push_back(cc);
    return *cc;
}
Constraint &Variable::addConstraint(int type, float scale, Variable *v, float scale2, Variable *v2)
{
    assert(v->id>=0);
    assert(v2->id>=0);
    auto *cc = new Constraint{this, {v,v2}, {scale,scale2}, type, 1,0,(int)c.size()};
    c.push_back(cc);
    return *cc;
}
void Variable::addConstraint(int type,
        float s1, Variable *v1,
        float s2, Variable *v2,
        float s3, Variable *v3)
{
    assert(v1->id>=0);
    assert(v2->id>=0);
    assert(v3->id>=0);
    c.push_back(new Constraint{this, {v1,v2,v3}, {s1,s2,s3}, type, 1,0,(int)c.size()});
}

Constraint &Variable::addConstraint(int type)
{
    auto *cc = new Constraint{this, {}, {}, type, 1,0,(int)c.size()};
    c.push_back(cc);
    return *cc;
}

void Variable::solveReallyBasicAlgebra()
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
bool Variable::redundantBound(Constraint &a, Constraint &b)
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

void Variable::removeRedundantBounds()
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

void Variable::trySolve()
{
    if(solved)
        return;
    //printf("TRY TO SOLVE MEEEEEEEEEEEEEEEEEEEEEE<%d>\n", id);
    for(int i=0; i<(int)c.size(); ++i) {
        auto &cc = *c[i];
        //printf("type = %d\n", cc.type);
        //printf("size = %d\n", cc.v.size());
        //printf("id   = %d\n", cc.v[0]->id);
        //printf("something %f\n", cc.scale[0]);
        assert(cc.v.size() == cc.scale.size());
        if(cc.type == 0 && cc.v.size() == 1 && cc.v[0]->id == 0)
            solve(cc.scale[0]);
        else if(cc.type == 0 && cc.v.size() == 1 && cc.v[0]->solved)
            solve(cc.scale[0]*cc.v[0]->solution);
        else if(cc.type == CONSTRAINT_LE && cc.v.size() == 1 && cc.v[0]->id == 0 && cc.scale[0] == 0)
            solve(0);
    }
}

void Variable::maximize()
{
    //In case where there is only one usable max rule
    //consisting of a constant and lower priority variables
    Constraint *rule = NULL;
    Constraint *unhandled_eq = NULL;
    for(int i=0; i<(int)c.size(); ++i) {
        if(c[i]->type == 1 && c[i]->v[0]->id==0) {
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

    if(!rule){// || unhandled_eq)
        printf("Failing to solve\n");
        return;
    }
    printf("Attempting solve\n");
    for(int i=0; i<rule->v.size(); ++i) {
        if(rule->v[i]->id == 0) {
            printf("Performing Soultino\n");
            solve(rule->scale[i]);
            break;
        }
    }

}

bool Variable::hasScalarEquality()
{
    Constraint *rule = NULL;
    for(int i=0; i<(int)c.size(); ++i)
        if(c[i]->type == 0 && c[i]->v.size() == 1)
            rule = c[i];
    if(!rule)
        return false;
    return true;
}

void Variable::inverseRewrite(float scale, Variable *var, int ref)
{
    //printf("inverse rewrite(%d,%d)\n",priority, var->priority);
    if(priority < var->priority)
        priority = var->priority;
    for(int i=var->c.size()-1; i>=0; --i) {
        auto &cc = *var->c[i];
        //cc.dump("old");
        if(cc.type != CONSTRAINT_EQ) {
            //printf("HERER\n");
            auto *ncon = new Constraint{this, {}, {}, cc.type, 1,0,(int)c.size()};
            for(int j=0; j<cc.v.size(); ++j) {
                //printf("tick...\n");
                ncon->v.push_back(cc.v[j]);
                ncon->scale.push_back(scale*cc.scale[j]);
            }
            //ncon->dump("rewritten");
            c.push_back(ncon);
            ncon->dedup();
            var->c.erase(var->c.begin()+i);
        } else if(cc.isScalarEquality() && cc.v[0]->id != ref){
            //printf("hazard on (%d)...\n",ref);
            addConstraint(CONSTRAINT_EQ, scale*cc.scale[0], cc.v[0]);
            //c[c.size()-1]->dump("new");
            var->c.erase(var->c.begin()+i);
        }
        cc.dedup();
    }
}

void Variable::rewrite(Variable *var, int ref)
{
    //printf("Rewrite(using %%%d on %%%d due to scalar on %%%d)...\n",id,var->id, ref);
    Constraint *rule = NULL;
    for(int i=0; i<(int)c.size(); ++i) {
        if(c[i]->type == 0 && c[i]->v.size() == 1) {
            rule = c[i];
            break;
        }
    }
    if(!rule)
        return;
    //else
    //    printf("Found Rule...\n");
    Variable *rep    = rule->v[0];
    float rep_scale =  rule->scale[0];
    //printf("rewrier = %%%d %p %p\n", rep->id, var, rep);
    if(rep == var)
        var->inverseRewrite(1/rep_scale, this, ref);
    //else
    {
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

void Variable::printName(bool force)
{
    assert(id >= 0);

    if(id==0 && force) {
        printf("1");
    } else if(id != 0) {
        printf("%%%d", id);
    }
}

void Variable::dump(const char *prefix)
{
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

float Variable::hasNegCorr(Variable *var)
{
    for(auto cc:c)
        for(int i=0; i<cc->v.size(); ++i)
            if(var == cc->v[i] && sign(cc->scale[i]) == -1)
                return true;
    return false;
}
float Variable::hasPosCorr(Variable *var)
{
    for(auto cc:c)
        for(int i=0; i<cc->v.size(); ++i)
            if(var == cc->v[i] && sign(cc->scale[i]) == +1)
                return true;
    return false;
}

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
        //cc.dedup();
    }
}

void Variable::clear()
{
    c.clear();
    fixed_value = 0;
    is_fixed    = 0;
    solution    = 0;
    solved      = 0;
    priority    = 1;
    id          = 10101;
    alias       = 0;
}

void Variable::operator=(LinearConstraint lc)
{
    lc.finalize();
    auto *cc = new Constraint{this, lc.v, lc.s, CONSTRAINT_EQ, 1,0,(int)c.size()};
    c.push_back(cc);
}
void Variable::operator<=(LinearConstraint lc)
{
    lc.finalize();
    auto *cc = new Constraint{this, lc.v, lc.s, CONSTRAINT_LE, 1,0,(int)c.size()};
    c.push_back(cc);
}
void Variable::operator>=(LinearConstraint lc)
{
    lc.finalize();
    auto *cc = new Constraint{this, lc.v, lc.s, CONSTRAINT_GE, 1,0,(int)c.size()};
    c.push_back(cc);
}

//{
//    assert(size_ == vec.size());
//    for(size_t i=0; i!=size_; ++i)
//        data_[i] = vec[i];
//    return *this;
//}

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
 *                   Bounding Box                                 *
 *****************************************************************/
void BBox::clear()
{
    x.clear();
    y.clear();
    w.clear();
    h.clear();
}

void BBox::dumpVar(Variable &var)
{
    if(var.solved)
        printf("%f",var.solution);
    else
        printf("?");
}
void BBox::dump(const char *prefix)
{
    printf("%s%s%d - <", prefix,prefix,id);
    dumpVar(x);printf(",");
    dumpVar(y);printf(",");
    dumpVar(w);printf(",");
    dumpVar(h);printf(">\n");
}

/******************************************************************
 *                   Layout Problem                               *
 *****************************************************************/

void LayoutProblem::addVariable(Variable *v)
{
    v->id = var.size()+1;
    var.push_back(v);
}
        
Variable *LayoutProblem::getNamedVariable(const char *name,
        int prior)
{
    for(int i=0;i<(int)var.size(); ++i)
        if(var[i]->name == name)
            return var[i];

    //Create Var
    Variable *var = new Variable;
    var->name     = name;
    var->priority = prior;
    addVariable(var);
    return var;
}

void LayoutProblem::addBoxVars()
{
    printf("Inserting Boxes...\n");
    for(unsigned i=0; i<box.size(); ++i) {
        auto &b = *box[i];
        printf("Parent = %p\n", b.parent);
        b.x.priority = 100;
        b.y.priority = 100;
        b.w.priority = 200;
        b.h.priority = 200;
        b.x.name = "x";
        b.y.name = "y";
        b.w.name = "w";
        b.h.name = "h";
        if(b.parent) {
            auto &p = *b.parent;
            //Lower Bounds
            b.x.addConstraint(CONSTRAINT_GE, 0, Const);
            b.y.addConstraint(CONSTRAINT_GE, 0, Const);
            b.w.addConstraint(CONSTRAINT_GE, 0, Const);
            b.h.addConstraint(CONSTRAINT_GE, 0, Const);

            //Upper Bounds
            b.x <= p.w - b.w;
            b.y <= p.h - b.h;
            b.w <= p.w - b.x;
            b.h <= p.h - b.y;
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

    //printf("  ");
    //for(int k=0; k<N; ++k)
    //    printf("%2d",k+1);
    //printf("\n");
    //for(int j=0; j<N; ++j) {
    //    printf("%2d",j+1);
    //    for(int k=0; k<N; ++k) {
    //        int c = corr_matrix[j*N+k];
    //        printf(c==-1?" -":c==1?" +":(c==2||c==-2)?" ?":" _");
    //    }
    //    printf("\n");
    //}
    //goto giant_hack;

    for(int i=0; i<N; ++i) {
        for(int j=0; j<N; ++j) {
            int tmp = corr_matrix[i*N+j];
            //Now we have the directed relationship of variable
            //i directed onto j + is good, - is bad, ? is confusion,
            //_ is no relationship
            //
            //if this directed relationship is not _, then
            //find all j directed onto k and create the 
            //i onto k relationship which corresponds to the merge
            //eg i (+) j and j (+) k implies i (+) k
            //printf("Operating on linkage [%d,%d]\n", i+1,j+1);
            if(tmp == 1 || tmp == -2 || tmp == 2) {
                for(int k=0; k<N; ++k) {
                    if(i==k)
                        continue;
                    //printf("normal corr [%d,%d] onto [%d,%d]=",j+1,k+1,i+1,k+1);
                    corr_matrix[i*N+k] =
                        confuse_merge(corr_matrix[i*N+k], corr_matrix[j*N+k]);
                    //printf("%d\n", corr_matrix[i*N+k]);
                }
                //corr_matrix[j*N+i] = corr_matrix[i*N+j] =
                //    confuse_merge(corr_matrix[i*N+j], corr_matrix[j*N+i]);
            } else if(tmp == -1) {
                for(int k=0; k<N; ++k) {
                    if(i==k)
                        continue;
                    //printf("neg corr    [%d,%d] onto [%d,%d]=",j+1,k+1,i+1,k+1);
                    corr_matrix[i*N+k] =
                        confuse_merge(corr_matrix[i*N+k], -corr_matrix[j*N+k]);
                    //corr_matrix[k*N+i] = corr_matrix[i*N+k] =
                    //    confuse_merge(corr_matrix[i*N+k], corr_matrix[k*N+i]);
                    //printf("%d\n", corr_matrix[i*N+k]);
                }
            }
        }
        //printf("  ");
        //for(int k=0; k<N; ++k)
        //    printf("%2d",k+1);
        //printf("\n");
        //for(int j=0; j<N; ++j) {
        //    printf("%2d",j+1);
        //    for(int k=0; k<N; ++k) {
        //        int c = corr_matrix[j*N+k];
        //        printf(c==-1?" -":c==1?" +":(c==2||c==-2)?" ?":" _");
        //    }
        //    printf("\n");
        //}
    }

    //printf("  ");
    //for(int k=0; k<N; ++k)
    //    printf("%2d",k+1);
    //printf("\n");
    //for(int j=0; j<N; ++j) {
    //    printf("%2d",j+1);
    //    for(int k=0; k<N; ++k) {
    //        int c = corr_matrix[j*N+k];
    //        printf(c==-1?" -":c==1?" +":(c==2||c==-2)?" ?":" _");
    //    }
    //    printf("\n");
    //}

giant_hack:
    for(int i=0; i<N; ++i) {
        if(var[i]->solved || var[i]->is_fixed)
            continue;
        int prior = var[i]->priority;
        int good  = true;
        for(int j=0; j<N; ++j) {
            int c = corr_matrix[i*N+j];
            if(c)
                printf("var %%%d(%d) affects %%%d(%d) in a %d manner\n",
                        var[i]->id,prior,var[j]->id,var[j]->priority, c);
            if((c==-1 || c==2 || c==-2) && var[j]->priority >= prior)
                good = false;
        }
        printf("Variable %%%d maxable: %d\n", var[i]->id, good);
        if(good)
            var[i]->maximize();
    }
}

void simplexResult(float *data, int rows, int cols, float *sol)
{
    float results[cols-1];
    memset(results, 0, sizeof(results));
    for(int i=0; i<cols-1; ++i) {
        int sel = -1;
        for(int j=0; j<rows; ++j) {
            //printf("[%d,%d] = %d\n",i,j,sel);
            if(sel == -1 && data[i+j*cols] == 1.0)
                sel = j;
            else if(sel != -1 && data[i+j*cols] == 1.0) {
                sel = -1;
                break;
            } else if(data[i+j*cols] != 0.0) {
                sel = -1;
                break;
            }
        }
        if(sel != -1) {
            //printf("sel good[%d]\n", sel);
            results[i] = data[(cols-1)+sel*cols];
        }
    }
    printf("solution:   [");
    for(int k=0; k<cols-1; ++k) {
        float v = results[k];
        if(fabs(v) > 1e-3)
            printf("%8.2f%s",v,k==cols-2?"]\n":",");
        else
            printf("        %s",k==cols-2?"]\n":",");
    }
    if(sol)
        for(int i=0; i<cols-1;++i)
            sol[i] = results[i];

}

void simplexCheck(float *data, float *orig, int rows, int cols)
{
    //solve the current and compare to the original matrix
    float new_solution[cols-1];
    simplexResult(data, rows, cols, new_solution);
    float old_b[rows-1];
    float new_b[rows-1];
    for(int i=0; i<rows-1; ++i)
        old_b[i] = orig[(cols-1)+i*cols];
    
    for(int i=0; i<rows-1; ++i) {
        new_b[i] = 0;
        for(int j=0; j<cols-1; ++j)
            new_b[i] += new_solution[j]*orig[j+i*cols];
    }

    printf("Verification:\n");
    printf("old: [");
    for(int k=0; k<rows-1; ++k) {
        float v = old_b[k];
        if(fabs(v) > 1e-3)
            printf("%8.2f%s",v,k==rows-2?"]\n":",");
        else
            printf("        %s",k==rows-2?"]\n":",");
    }
    printf("new: [");
    for(int k=0; k<rows-1; ++k) {
        float v = new_b[k];
        if(fabs(v) > 1e-3)
            printf("%8.2f%s",v,k==rows-2?"]\n":",");
        else
            printf("        %s",k==rows-2?"]\n":",");
    }
    for(int k=0; k<rows-1; ++k)
        assert(fabs(new_b[k]-old_b[k]) < 1e-4);


}

void simplexRescale(float *data, int rows, int cols, int r, int c)
{
    const float pv = data[c+r*cols];
    for(int i=0; i<cols; ++i)
        data[i+r*cols] /= pv;
}

void simplexPerformPivot(float *data, int rows, int cols, int r, int c)
{
    simplexRescale(data,rows, cols, r, c);
    //ok, so the row is static
    //eliminate this variable from other rows via gauss elim
    const float pv = data[c+r*cols];
    for(int i=0; i<rows; ++i) {
        if(i==r)
            continue;
        float scale = data[c+i*cols]/pv;
        for(int j=0; j<cols; ++j)
            data[j+i*cols] -= scale*data[j+r*cols];
    }
}

void simplexColPow(float *data, int rows, int cols, int c, float &pow, int &idx, float *vvv=NULL)
{
    printf("Pivot Col = %d\n", c);
    float pivot_value = 1.0/0.0;//Inf
    int selected_row = -1;
    for(int i=0; i<rows-1; ++i) {
        float b   = data[cols-1+i*cols];
        float val = data[c+i*cols];
        float ratio = b/val;
        //printf("ratio = %f<%f/%f>", ratio,b,val);
        if(ratio > 0  && ratio < pivot_value) {
            //printf("*\n");
            selected_row = i;
            pivot_value  = val;
            if(vvv)
                *vvv = ratio;
        } else;
            //printf("\n");
    }
    if(selected_row == -1) {
        for(int i=0; i<rows-1; ++i) {
            float b   = data[cols-1+i*cols];
            float val = data[c+i*cols];
            float ratio = b/val;
            //printf("ratio = %f<%f/%f>", ratio,b,val);
            if(((ratio > 0 || b==0) && b>=0 && ratio < pivot_value)) {
                //printf("*\n");
                selected_row = i;
                pivot_value  = val;
                if(vvv)
                    *vvv = ratio;
            } else;
            //printf("\n");
        }
    }
    printf("Pivot row = %d\n", selected_row);
    if(selected_row != -1) {
        assert(pivot_value > 0);
        idx = selected_row;
        pow = pivot_value;
    } else {
        printf("POSSIBLY UNBOUNDED\n");
        //for(int i=0; i<rows-1; ++i) {
        //    float b   = data[cols-1+i*cols];
        //    float val = data[c+i*cols];
        //    float ratio = b/val;
        //    printf("ratio = %f<%f/%f>", ratio,b,val);
        //    if((ratio == 0 || ratio == -0) && val!=0) {
        //        printf("*\n");
        //        printf("Performing recovery...\n");
        //        idx = i;
        //        pivot_value  = 0;
        //        return;
        //    } else printf("\n");
        //}
        idx = -1;
        pivot_value = 0;
    }
}

bool simplexPivotConstraints(float *data, int rows, int cols)
{
    printf("Verifying B>=0\n");
    int sel_row = -1;
    for(int i=0; i<rows-1; ++i) {
        if(data[(cols-1)+i*cols] < 0) {
            sel_row = i;
            break;
        }
    }
    printf("B>=0 row %d\n", sel_row);
    if(sel_row == -1)
        return false;
    for(int i=0; i<cols; ++i) {
        data[i+sel_row*cols] *= -1;
    }
    return true;
}

bool simplexPivot(float *data, int rows, int cols, int *pivoted)
{
    if(simplexPivotConstraints(data, rows, cols))
        return true;

    int selected_col=-1;
    float selected_val=-1e-9;
    for(int i=0; i<cols-1; ++i) {
        float val = data[i+(rows-1)*cols];
        //if(val < 0) {
        //    selected_val = val;
        //    selected_col = i;
        //    //break;
        //}
        if(val < selected_val) {
            selected_val = val;
            selected_col = i;
            //int cc=-1;
            //float vv=0;
            //float vvv=0;
            //simplexColPow(data, rows, cols, i, vv, cc, &vvv);
            //if(cc != -1 && (selected_val != -1 || (selected_col == -1 && vvv >0))) {
            //    printf("new leader<%f,%d>\n", vv, i);
            //    selected_val = val;
            //    selected_col = i;
            //}
        }
    }
    printf("Objective value = %f\n", selected_val);
    if(selected_col<0)
        return false;
    float pivot_value = 1.0/0.0;//Inf
    int selected_row = -1;
    float vvv = 0;
    simplexColPow(data, rows, cols, selected_col, pivot_value, selected_row, &vvv);

    if(selected_row < 0)
        return false;
        //assert(false);
    pivoted[selected_col] = selected_row;
    printf("Selected pivot = [%d,%d]\n", selected_row, selected_col);
    printf("Pivot Value = %f<%f>\n", pivot_value,vvv);
    simplexPerformPivot(data, rows, cols, selected_row, selected_col);
    return true;
}

#include "lp-solve.cpp"

void LayoutProblem::passSimplex()
{
    printf("Simplex Pass\n");
    const unsigned N=var.size();
    int priority = 0;
    //Find Maximum Priority Variable Class
    for(int i=0; i<N; ++i)
        if(!var[i]->is_fixed && !var[i]->solved)
            if(var[i]->priority > priority)
                priority = var[i]->priority;
    printf("Priority = %d\n", priority);
    bool activeVariable[N+1];
    unsigned M=0;//Active Variables
    unsigned L=0;//Variables That Exist
    for(int i=0; i<N; ++i) {
        if(!var[i]->is_fixed && !var[i]->solved &&
            var[i]->priority == priority) {
            var[i]->dump("  ");
            activeVariable[i] = true;
            M++;
        } else
            activeVariable[i] = false;
        if(!var[i]->is_fixed && !var[i]->solved)
            L++;
    }

    int rewrite[N+1];
    {
        int id=1;
        rewrite[0] = 0;
        for(int i=0; i<N; ++i) {
            rewrite[var[i]->id] = id;
            if(!var[i]->is_fixed && !var[i]->solved && var[i]->id != 0)
                id++;
        }
        //printf("Variables = %d+1\n", L);
        //for(int i=0;i<N+1;++i)
        //    printf("rewrite[%d] = %d<%d>\n", i, rewrite[i], i==0?-1:activeVariable[i-1]);
    }


    std::vector<std::vector<float>> constraints;
    std::vector<bool> has_slack;
    int slack_vars = 0;
    for(int i=0; i<N; ++i) {
        if(var[i]->is_fixed || var[i]->solved)
            continue;
        printf("Variable %%%d\n", var[i]->id);
        auto &v = *var[i];
        for(int j=0; j<v.c.size(); ++j) {
            std::vector<float> rc;
            rc.resize(L+1);
            for(int i=0;i<rc.size();++i)
                rc[i] = 0;
            auto &c = *v.c[j];
            if(c.type == CONSTRAINT_GE)
                continue;
            c.dump("con");
            rc[rewrite[var[i]->id]] = -1;
            for(int k=0; k<c.size(); ++k) {
                printf("%d?\n",c.v[k]->id);
                assert(rewrite[c.v[k]->id] >=0);
                assert(rewrite[c.v[k]->id] < rc.size());
                rc[rewrite[c.v[k]->id]] += c.scale[k];
            }
            for(int k=1; k<rc.size(); ++k)
                rc[k] *= -1;
            bool doSkip = false;
            for(auto t:constraints) {
                float diff = 0;
                for(int k=0; k<rc.size(); ++k)
                    diff += fabs(t[k]-rc[k]);
                if(diff < 0.01)
                    doSkip = true;
            }
            if(doSkip)
                continue;
            if(c.type == CONSTRAINT_LE) {
                has_slack.push_back(true);
                slack_vars++;
            } else {
                has_slack.push_back(false);
            }
            printf("constraint: [");
            for(int k=0; k<rc.size(); ++k)
                printf("%f%s",rc[k],k==rc.size()-1?"]\n":",");
            constraints.push_back(rc);
        }
    }
    if(constraints.size()==0)
        return;
    //Build Always True Constraint
    if(0){
        std::vector<float> rc;
        rc.resize(L+1);
        rc[0] = 0;
        for(int i=0; i<L; ++i)
            rc[i+1] = -1;
        slack_vars++;
        has_slack.push_back(true);
        constraints.push_back(rc);
    }


    //Construct The Simplex Table
    int   rows = constraints.size()+2;
    int   cols = slack_vars+L+1;
    Matrix simplexTable(rows, cols);
    int slacky = 0;
    for(int i=0; i<rows-2; ++i) {
        if(has_slack[i])
            simplexTable(i, L+(slacky++)) = 1;
        //assert(constraints[i][0]>=-0.1);
        simplexTable(i,cols-1) = constraints[i][0];
        for(int j=0; j<L; ++j)
            simplexTable(i,j) = constraints[i][j+1];
    }
    for(int i=0; i<N; ++i) {
        int j = var[i]->id;
        if(var[i]->priority == priority && !var[i]->is_fixed) {
            //printf("rewrite[%d]=%d\n", j, rewrite[j]);
            assert(rewrite[j]>0);
            simplexTable(rows-2,rewrite[j]-1) = -1;
        }
    }


    //Matrix test(5,7);
    //float test_data[] = {2,5,3,-1,0,0,185,3,2.5,8,0,-1,0,155,8,10,4,0,0,-1,600,4,8,3,0,0,0};
    //for(int i=0;i<sizeof(test_data)/4;++i)
    //    test.data[i] = test_data[i];
    //rows = test.rows;
    //cols = test.cols;
    for(int i=0; i<cols; ++i)
        for(int j=0; j<rows-2; ++j)
            simplexTable(rows-1,i) -= simplexTable(j,i);

    printf("LP Input...\n");
    //simplexTable.dump();
    LpResults sol = linear_program(simplexTable);
    assert(sol.is_solved);
    for(int i=0; i<N; ++i) {
        int j=var[i]->id;
        if(rewrite[j]>0 && var[i]->priority == priority && !var[i]->is_fixed && !var[i]->solved) {
            //printf("%%%d->%d->%f\n", j, rewrite[j]-1, sol.solution(rewrite[j]-1,0));
            var[i]->solve(sol.solution(rewrite[j]-1,0));
        }
    }
    //exit(1);


#if 0
    for(int i=0; i<constraints.size(); ++i) {
        auto &c=constraints[i];
        float first_one=0;
        for(int j=1; j<c.size(); ++j) {
            if(first_one != 0)
                c[j] /= first_one;
            else if(c[j] != 0) {
                first_one = fabs(c[j]);
                c[j] /= first_one;
            }
        }
        if(first_one != 0)
            c[0] /= first_one;
    }

    //XXX XXX
    //Todo normalize the constraints and kill off the loose constraints
    

    //1 row per constraint and 1 for the objective
    //1 col per variable 1 col per slack 1 col per upper bound
    int   rows = constraints.size()+1;
    int   cols = slack_vars+L+1;
    float *matrix_data = new float[rows*cols];
    memset(matrix_data, 0, rows*cols*sizeof(float));
    int slacky = 0;
    for(int i=0; i<rows-1; ++i) {
        if(has_slack[i])
            matrix_data[L+(slacky++)+i*cols] = 1;
        assert(constraints[i][0]>=0);
        matrix_data[cols-1+i*cols] = constraints[i][0];
        for(int j=0; j<L; ++j)
            matrix_data[j+i*cols] = constraints[i][j+1];
    }
    int rebuild[N];
    //for(int i=0; i<L; ++i) {
    //    matrix_data[i+(rows-1)*cols] = -0.01;
    //}
    for(int i=0; i<N; ++i) {
        int j = var[i]->id;
        if(var[i]->priority == priority && !var[i]->is_fixed) {
            printf("rewrite[%d]=%d\n", j, rewrite[j]);
            assert(rewrite[j]>0);
            matrix_data[rewrite[j]-1+(rows-1)*cols] = -1;
            printf("col %d is the rewrite of var %%%d\n", rewrite[j]-1, j);
            rebuild[i] = rewrite[i]-1;
        } else {
            rebuild[i] = -1;
            //if(matrix_data[rewrite[i]-1+(rows-1)*cols] == 0)
            //    matrix_data[rewrite[i]-1+(rows-1)*cols] = -0.01;
        }
    }

    float *matrix_orig = new float[rows*cols];
    for(int i=0; i<rows*cols; ++i)
        matrix_orig[i] = matrix_data[i];

    int pivoted[rows];
    for(int i=0; i<rows; ++i)
        pivoted[i] = -1;

    printf("Simplex Initialization:\n");
    for(int i=-1; i<rows; ++i) {
        printf("constraint: [");
        for(int k=0; k<cols; ++k) {
            float v = i==-1?k:matrix_data[k+i*cols];
            if(fabs(v) > 1e-3)
                printf("%8.2f%s",v,k==cols-1?"]\n":",");
            else
                printf("        %s",k==cols-1?"]\n":",");
        }
    }
    simplexCheck(matrix_data, matrix_orig, rows, cols);
    //simplexResult(matrix_data, rows, cols, NULL);

    for(int _=0;_<100;++_) {
        printf("Simplex Pivot#%d:\n",_);
        if(!simplexPivot(matrix_data, rows, cols, pivoted))
            break;
        for(int i=-1; i<rows; ++i) {
            printf("constraint: [");
            for(int k=0; k<cols; ++k) {
                float v = i==-1?k:matrix_data[k+i*cols];
                if(fabs(v) > 1e-3)
                    printf("%8.2g%s",v,k==cols-1?"]\n":",");
                else
                    printf("        %s",k==cols-1?"]\n":",");
            }
        }
        simplexCheck(matrix_data, matrix_orig, rows, cols);
        //simplexResult(matrix_data, rows, cols);
    }
    printf("Solutions:\n");
    for(int i=0; i<rows; ++i) {
        if(pivoted[i] != -1) {
            printf("x_%d = %f\n", i, matrix_data[cols-1+pivoted[i]*cols]);
        }
    }
    printf("Pivots:\n");
    for(int i=0; i<rows; ++i)
        if(pivoted[i] != -1)
            printf("<%d,%d>",i,pivoted[i]);
    for(int i=0; i<N; ++i)
    {
        if(var[i]->is_fixed || var[i]->solved)
            continue;
        //int search = rewrite[i];
        if(rebuild[i] != -1 && pivoted[rebuild[i]] != -1) {
            printf("%%%d <= index <<%d,%d>> [%d,%d]\n", var[i]->id, i, rebuild[i], cols-1, pivoted[rebuild[i]]);
            float val = matrix_data[cols-1+pivoted[rebuild[i]]*cols];
            if(val==val && val < 1e9)
                var[i]->solve(val);
            printf("%%%d = %f\n", var[i]->id, val);
        }
    }
#endif

}

/**
 * Ok, none of the simple math works, so find the maximum priority set of
 * variables
 *
 * Translate the problem into 
 *
 * Maximize \sum_{\forall i} v_i^2
 *
 * Given \forall i v_i >= 0
 * Subject to Av <= c
 */ 
#if 0
void LayoutProblem::passSimplex()
{
    printf("Simplex Pass\n");
    const unsigned N=var.size();
    int priority = 0;
    //Find Maximum Priority Variable Class
    for(int i=0; i<N; ++i)
        if(!var[i]->is_fixed && !var[i]->solved)
            if(var[i]->priority > priority)
                priority = var[i]->priority;
    printf("Priority = %d\n", priority);
    bool activeVariable[N+1];
    int  M=0;
    for(int i=0; i<N; ++i) {
        if(!var[i]->is_fixed && !var[i]->solved &&
            var[i]->priority == priority) {
            var[i]->dump("  ");
            activeVariable[var[i]->id] = true;
            M++;
        } else
            activeVariable[var[i]->id] = false;
    }

    int rewrite[N+1];
    int id=0;
    rewrite[0] = 0;
    for(int i=0; i<N+1; ++i) {
        if(activeVariable[i])
            id++;
        rewrite[i] = id;
    }
    for(int i=0;i<N;++i)
        printf("rewrite[%d] = %d<%d>\n", i, rewrite[i], activeVariable[i]);


    std::vector<std::vector<float>> constraints;
    std::vector<std::vector<float>> rc;
    for(int i=0; i<N; ++i) {
        if(!activeVariable[i])
            continue;
        auto &v = *var[i];
        //v.dump("real  ");
        for(int j=0; j<v.c.size(); ++j) {
            //printf("v.c[%d]\n", j);
            //v.c[j]->dump("  ");
            //fflush(stdout);
            auto &c = *v.c[j];
            if(c.type == CONSTRAINT_GE)
                continue;
            rc.clear();
            rc.push_back(std::vector<float>());
            rc[0].resize(M+1);
            for(int k=0; k<M+1; ++k)
                rc[0][k] = 0;
            rc[0][rewrite[i]] = -1;
            //printf("constraint: [");
            //for(int k=0; k<M+1; ++k)
            //    printf("%f%s",constraint[k],k==M?"]\n":",");
            for(int k=0; k<c.size(); ++k) {
                std::vector<std::vector<float>> rc_next;
                for(auto cur:rc) {
                    //printf("adding' <%d>(%d)\n", c.v[k]->id, rewrite[c.v[k]->id]);
                    if(activeVariable[c.v[k]->id] || c.v[k]->id == 0) {
                        //printf("non-recursive\n");
                        cur[rewrite[c.v[k]->id]] += c.scale[k];
                        rc_next.push_back(cur);
                    } else {
                        //c.dump("recursive");
                        //c.v[k]->dump("rvar ");
                        auto res = recurseConstraint(c.v[k]->id, (float)c.scale[k],
                                (bool*)activeVariable, (int*)rewrite, cur);
                        for(auto x:res)
                            rc_next.push_back(x);
                    }
                }
                rc = rc_next;
            }
            //printf("constraint: [");
            //for(int k=0; k<M+1; ++k)
            //    printf("%f%s",constraint[k],k==M?"]\n":",");
            //printf("checking <%d>(%d)\n", i, rewrite[i]);
            //printf("constraint[rewrite[%d]] == %f\n", i, constraint[rewrite[i]]);
            for(auto cur:rc) {
                for(int k=1; k<M+1; ++k)
                    cur[k] *= -1;
                //printf("constraint: [");
                //for(int k=0; k<M+1; ++k)
                //    printf("%8.2f%s",cur[k],k==M?"]\n":",");
                constraints.push_back(cur);
            }
        }
    }

    int   rows = constraints.size()+1;
    int   cols = constraints.size()+M+1;
    float *matrix_data = new float[rows*cols];
    memset(matrix_data, 0, rows*cols*sizeof(float));
    for(int i=0; i<rows-1; ++i) {
        matrix_data[M+i+i*cols] = 1;
        matrix_data[cols-1+i*cols] = constraints[i][0];
        for(int j=0; j<M; ++j)
            matrix_data[j+i*cols] = constraints[i][j+1];
    }
    for(int i=0; i<M; ++i)
        matrix_data[i+(rows-1)*cols] = -1;
    int pivoted[rows];
    for(int i=0; i<rows; ++i)
        pivoted[i] = -1;

    printf("Simplex Initialization:\n");
    for(int i=0; i<rows; ++i) {
        printf("constraint: [");
        for(int k=0; k<cols; ++k)
            printf("%8.2f%s",matrix_data[k+i*cols],k==cols-1?"]\n":",");
    }

    for(int _=0;_<10;++_) {
        printf("Simplex Pivot:\n");
        if(!simplexPivot(matrix_data, rows, cols, pivoted))
            break;
        for(int i=0; i<rows; ++i) {
            printf("constraint: [");
            for(int k=0; k<cols; ++k)
                printf("%8.2f%s",matrix_data[k+i*cols],k==cols-1?"]\n":",");
        }
    }
    printf("Solutions:\n");
    for(int i=0; i<rows; ++i) {
        if(pivoted[i] != -1) {
            printf("x_%d = %f\n", i, matrix_data[cols-1+pivoted[i]*cols]);
        }
    }

    fflush(stdout);
    exit(1);
}
#endif

//XXX todo make this function complete (include <= bounds)
std::vector<std::vector<float>> LayoutProblem::recurseConstraint(int var_, float gain, bool *active, int *rw,
        std::vector<float> constraint)
{
    std::vector<std::vector<float>> res;
    res.push_back(constraint);
    //printf("constraint: [");
    //for(int k=0; k<constraint.size(); ++k)
    //    printf("%f%s",constraint[k],k==constraint.size()-1?"]\n":",");
    //printf("recurseConstraint(%d)...\n", var_);
    auto &v = *var[var_-1];
    assert(v.id == var_);
    if(v.solved || v.is_fixed) {
        return res;
    }
    //v.dump("asdf");
    for(int j=0; j<v.c.size(); ++j) {
        auto &c = *v.c[j];
        if(c.type == CONSTRAINT_GE) 
            continue;
        for(int k=0; k<c.size(); ++k) {
            std::vector<std::vector<float>> rc_next;
            for(auto cur:res) {
                //printf("adding'' <%d>(%d)\n", c.v[k]->id, rw[c.v[k]->id]);
                if(active[c.v[k]->id] || c.v[k]->id == 0) {
                    //printf("non-recursive\n");
                    //c.dump("*non");
                    //printf("old=%f\n", cur[rw[c.v[k]->id]]);
                    //cur[rw[c.v[k]->id]] += c.scale[k];
                    //printf("new=%f\n", cur[rw[c.v[k]->id]]);
                    rc_next.push_back(cur);
                } else {
                    //printf("recurse again");
                    //c.v[k]->dump("*rec ");
                    auto res = recurseConstraint(c.v[k]->id, (float)c.scale[k],
                            (bool*)active, (int*)rw, cur);
                    for(auto x:res)
                        rc_next.push_back(x);
                }
            }
            res = rc_next;
        }
    }
    //printf("returning %d constraints...\n", (int)res.size());
    return res;
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
        if(var[i]->solved || var[i]->is_fixed)
            continue;
        for(int j=0; j<var[i]->c.size(); ++j) {
            auto &cc = *var[i]->c[j];
            if(cc.isScalarEquality()) {
                printf("Trying to fix var %%%d to %%%d\n", var[i]->id,
                        cc.v[0]->id);
                var[i]->is_fixed = true;
                for(int j=0;j<(int)var.size(); ++j)
                    if(j!=i)
                        var[i]->rewrite(var[j], cc.v[0]->id);
                break;
            }
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
            //cc.dump("it");
            //printf("SOLVED THING<%d> [%d %d %d]\n", var[i]->id, cc.isScalarEquality(),  !cc.isConstOnly(), cc.type == CONSTRAINT_EQ);
            if(cc.isScalarEquality() && !cc.isConstOnly() && cc.type == CONSTRAINT_EQ) {
                cc.v[0]->addConstraint(CONSTRAINT_EQ,
                        1/cc.scale[0], var[i]);
                var[i]->c.erase(var[i]->c.begin()+j);
            } else if(cc.v.size() == 2 && cc.v[0]->id == 0 && cc.v[1]->id != 0 &&
                    cc.type == CONSTRAINT_LE && cc.scale[1] < 0)
            {
                //printf("Making a new Transfered Solution Constraint(%f)...\n",
                //        var[i]->solution);
                //cc.dump("OLD: ");
                cc.v[1]->addConstraint(CONSTRAINT_LE,
                        -(cc.scale[0]-var[i]->solution)/cc.scale[1], cc.v[0]);
                cc.v[1]->c[cc.v[1]->c.size()-1]->dump("NEW: ");
                var[i]->c.erase(var[i]->c.begin()+j);

            } else if(cc.v.size() == 2 && cc.v[0]->id == 0 && cc.v[1]->id != 0 &&
                    cc.type == CONSTRAINT_LE && cc.scale[1] >= 0) {
                //printf("Making a new Transfered Solution Constraint'(%f)...\n",
                //        var[i]->solution);
                //cc.dump("OLD: ");
                cc.v[1]->addConstraint(CONSTRAINT_LE,
                        (cc.scale[0]-var[i]->solution)/cc.scale[1], cc.v[0]);
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
        //Add more bounds
        float best = 1.0/0.0;
        bool hasBest = false;
        for(int j=0; j<var[i]->c.size(); ++j) {
            auto &cc = *var[i]->c[j];
            if(cc.isConstAndNeg() && cc.scale[0] < best) {
                hasBest = true;
                best = cc.scale[0];
            }
        }

        if(hasBest)
            *var[i] <= best;
        var[i]->removeRedundantBounds();
    }
}

void LayoutProblem::passNoNegativeUpper()
{
    for(int i=0; i<(int)var.size(); ++i) {
        if(var[i]->solved)
            continue;
        for(int j=0; j<var[i]->c.size(); ++j) {
            auto &cc = *var[i]->c[j];
            if(cc.isStrictlyNegativeUpper()) {
                var[i]->solve(0);
                for(int k=0;k<cc.v.size();++k)
                    if(cc.v[k]->id != 0)
                        cc.v[k]->solve(0);
            }
        }
    }

}

void LayoutProblem::solve()
{
    //Solve Trivial Variables
    //for(int i=0;i<5;++i) {
    //    passReduceWithSolved();
    //    passScalarVariableFixing();
    //    passTransferSolvedConstraints();
    //    passReduceWithSolved();
    //    passSolveTrivial();
    //    passTighten();
    //}
    
    passSolveTrivial();
    //passScalarVariableFixing();
    //Propigate Solutions
    passReduceWithSolved();
    //passInvertTrivialBounds();
    //passTransferSolvedConstraints();
    //passSolveTrivial();
    //passTighten();
    //passNoNegativeUpper();
    //printf("-------------------------------------------------\n");
    //printf("--------Uhhh Input-------------------------------\n");
    //printf("-------------------------------------------------\n");
    //dump();
    //for(int i=0; i<10; ++i)
    //    passScalarVariableFixing();
    //passScalarVariableFixing();
    //passSolveTrivial();
    //passDedup();
    //passTighten();

    printf("-------------------------------------------------\n");
    printf("--------First Dep Solve Input--------------------\n");
    printf("-------------------------------------------------\n");
    dump();
    passSimplex();
    dump();
    for(int i=0;i<5;++i) {
        passReduceWithSolved();
        passScalarVariableFixing();
        passTransferSolvedConstraints();
        passReduceWithSolved();
        passSolveTrivial();
        passTighten();
    }
    printf("-------------------------------------------------\n");
    printf("--------Second Dep Solve Input-------------------\n");
    printf("-------------------------------------------------\n");
    dump();
    passSimplex();
    dump();
    for(int i=0;i<5;++i) {
        passReduceWithSolved();
        passScalarVariableFixing();
        passTransferSolvedConstraints();
        passReduceWithSolved();
        passSolveTrivial();
        passTighten();
    }
    printf("-------------------------------------------------\n");
    printf("--------Third Dep Solve Input--------------------\n");
    printf("-------------------------------------------------\n");
    dump();
    passSimplex();
    for(int i=0;i<5;++i) {
        passReduceWithSolved();
        passScalarVariableFixing();
        passTransferSolvedConstraints();
        passReduceWithSolved();
        passSolveTrivial();
        passTighten();
    }
    passSimplex();
    for(int i=0;i<5;++i) {
        passReduceWithSolved();
        passScalarVariableFixing();
        passTransferSolvedConstraints();
        passReduceWithSolved();
        passSolveTrivial();
        passTighten();
    }
    dump();
    fflush(stdout);
    return;
    //exit(1);

    depSolve();

    //Propigate Solutions
    passReduceWithSolved();
    printf("-------------------------------------------------\n");
    printf("--------1.5 Depsolve results --------------------\n");
    printf("-------------------------------------------------\n");
    dump();
    passInvertTrivialBounds();
    passTransferSolvedConstraints();
    passScalarVariableFixing();
    passScalarVariableFixing();
    passScalarVariableFixing();
    passSolveTrivial();
    passSolveBasicAlgebra();
    passTransferSolvedConstraints();
    passDedup();
    passTighten();


    printf("-------------------------------------------------\n");
    printf("--------Second Dep Solve Input-------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();


    //Find any cases of scalar equality and eliminate redundant vars
    passScalarVariableFixing();
    passScalarVariableFixing();
    passTransferSolvedConstraints();
    passSolveBasicAlgebra();
    passDedup();
    passTighten();
    passReduceWithSolved();
    passDedup();


    printf("-------------------------------------------------\n");
    printf("---------Third Dep Solve Input-------------------\n");
    printf("-------------------------------------------------\n");
    dump();

    depSolve();

    //Propigate Solutions
    passReduceWithSolved();
    passScalarVariableFixing();
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
    printf("\tBoxes:     %d\n", (int)box.size());
    for(int i=0; i<(int)box.size(); ++i) {
        box[i]->dump("\t");
    }
    printf("\tVariables: %d\n", (int)var.size());
    for(int i=0; i<(int)var.size(); ++i) {
        if(var[i]->is_fixed || var[i]->solved)
            continue;
        var[i]->dump("\t");
    }
    fflush(stdout);
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
