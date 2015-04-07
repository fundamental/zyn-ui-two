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
        void addVar(float scale_, Variable &v_);
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

        Constraint &addConstraint(int type, float scale, Variable *v)
        {
            assert(v->id>=0);
            auto *cc = new Constraint{this, {v}, {scale}, type, 1,0,(int)c.size()};
            c.push_back(cc);
            return *cc;
        }
        void addConstraint(int type, float scale, Variable *v, float scale2, Variable *v2)
        {
            assert(v->id>=0);
            assert(v2->id>=0);
            c.push_back(new Constraint{this, {v,v2}, {scale,scale2}, type, 1,0,(int)c.size()});
        }
        void addConstraint(int type,
                float s1, Variable *v1,
                float s2, Variable *v2,
                float s3, Variable *v3)
        {
            assert(v1->id>=0);
            assert(v2->id>=0);
            assert(v3->id>=0);
            c.push_back(new Constraint{this, {v1,v2,v3}, {s1,s2,s3}, type, 1,0,(int)c.size()});
        }

        Constraint &addConstraint(int type)
        {
            auto *cc = new Constraint{this, {}, {}, type, 1,0,(int)c.size()};
            c.push_back(cc);
            return *cc;
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

            if(!rule)// || unhandled_eq)
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

class BBox
{
    public:
        int id=0;
        BBox *parent=0;
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

extern Variable *Const;

