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
        bool isStrictlyNegativeUpper();
        bool isConstAndNeg();
        void dedup();

        void dump(const char *prefix);

        bool isConstOnly();
};

class Variable;
class LinearConstraint
{
    public:
        LinearConstraint(Variable&);
        LinearConstraint(float);
        void finalize();
        std::vector<float> s;
        std::vector<Variable*> v;
};

LinearConstraint operator-(LinearConstraint, LinearConstraint);
LinearConstraint operator+(LinearConstraint, LinearConstraint);
LinearConstraint operator*(LinearConstraint, LinearConstraint);
LinearConstraint operator/(LinearConstraint, LinearConstraint);

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

        void solve(float s);

        Constraint &addConstraint(int type, float scale, Variable *v);
        Constraint &addConstraint(int type, float scale, Variable *v, float scale2, Variable *v2);
        void addConstraint(int type,
                float s1, Variable *v1,
                float s2, Variable *v2,
                float s3, Variable *v3);
        Constraint &addConstraint(int type);

        void solveReallyBasicAlgebra();

        //True if b is redundant if a exists
        bool redundantBound(Constraint &a, Constraint &b);

        void removeRedundantBounds();
        void trySolve();

        void maximize();

        void reduceWithSolved(Variable *var);

        bool hasScalarEquality();

        void inverseRewrite(float scale, Variable *var, int ref);

        void rewrite(Variable *var, int ref);

        void printName(bool force);

        void dump(const char *prefix);

        float hasNegCorr(Variable *var);
        float hasPosCorr(Variable *var);

        void clear();


        void operator=(Variable &v) {*this = LinearConstraint(v);};
        void operator=(LinearConstraint  lc);
        void operator<=(LinearConstraint lc);
        void operator>=(LinearConstraint lc);

        std::vector<Constraint *> c;
};

class BBox
{
    public:
        int id=0;
        BBox *parent=0;
        Variable x,y,w,h;

        void clear();
        void dumpVar(Variable &var);
        void dump(const char *prefix);
};

class LayoutProblem
{
    public:
        std::vector<BBox *>     box;
        std::vector<Variable *> var;
        void addVariable(Variable *var);
        Variable *getNamedVariable(const char *name, int prior);
        void addBoxVars();
        void solve();
        void depSolve();
        std::vector<std::vector<float>>
        recurseConstraint(int var, float gain,
                bool *active, int *rw, std::vector<float> constraint);
        void passSimplex();
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
        void passNoNegativeUpper();

        void check_solution();
        void dump();
        void addBox(BBox &b)
        {
            b.id = box.size();
            box.push_back(&b);
        }
};

extern Variable *Const;

