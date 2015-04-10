#include "constraint-layout.h"

//#define QUIET_DOWN
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
        if(cc.type == 0 && cc.v.size() == 1 && cc.v[0]->id == 0)
            solve(cc.scale[i]);
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
    printf("inverse rewrite(%d,%d)\n",priority, var->priority);
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
        cc.dedup();
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
    //printf("Inserting Boxes...\n");
    for(unsigned i=0; i<box.size(); ++i) {
        auto &b = *box[i];
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
    bool activeVariable[N];
    int  M=0;
    for(int i=0; i<N; ++i) {
        if(!var[i]->is_fixed && !var[i]->solved &&
            var[i]->priority == priority) {
            var[i]->dump("  ");
            activeVariable[i] = true;
            M++;
        } else
            activeVariable[i] = false;
    }


    float constraint[M+1];
    for(int i=0; i<N; ++i) {
        for(int j=0; j<M+1; ++j)
            constraint[j] = 0;

    }
    fflush(stdout);
    exit(1);
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
                //printf("Trying to fix var %%%d to %%%d\n", var[i]->id,
                //        cc.v[0]->id);
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
            cc.dump("it");
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

            } else if(cc.v.size() == 2 && cc.v[0]->id == 0 && cc.v[1]->id != 0 &&
                    cc.type == CONSTRAINT_LE && cc.scale[1] >= 0) {
                printf("Making a new Transfered Solution Constraint'(%f)...\n",
                        var[i]->solution);
                cc.dump("OLD: ");
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
    passSolveTrivial();

    //Propigate Solutions
    passReduceWithSolved();
    passInvertTrivialBounds();
    passTransferSolvedConstraints();
    passNoNegativeUpper();
    printf("-------------------------------------------------\n");
    printf("--------Uhhh Input-------------------------------\n");
    printf("-------------------------------------------------\n");
    dump();
    for(int i=0; i<10; ++i)
        passScalarVariableFixing();
    //passScalarVariableFixing();
    passSolveTrivial();
    passDedup();
    passTighten();

    printf("-------------------------------------------------\n");
    printf("--------First Dep Solve Input--------------------\n");
    printf("-------------------------------------------------\n");
    dump();

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
    passSimplex();

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
        if(var[i]->is_fixed)
            continue;
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
