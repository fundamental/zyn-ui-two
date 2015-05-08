#include <cassert>
#include <cstdio>
#include <cstring>
struct Matrix
{
    Matrix(int r=1, int c=1) {
        rows=r;
        cols=c;
        data = new float[r*c];
        memset(data,0,r*c*sizeof(float));
    }
    Matrix(const Matrix &m)
    {
        rows = m.rows;
        cols = m.cols;
        data = new float[rows*cols];
        for(int i=0; i<rows*cols; ++i)
            data[i] = m.data[i];
    }
    void delRow(int r)
    {
        for(int i=r; i<rows-1; ++i)
            for(int j=0; j<cols; ++j)
                (*this)(i,j) = (*this)(i+1,j);
        rows--;
    }
    
    void delCol(int c)
    {
        float *nd=new float[rows*(cols-1)];
        for(int i=0; i<rows; ++i) {
            for(int j=0; j<cols; ++j) {
                if(j==c)
                    continue;
                else if(j<c)
                    nd[j+i*(cols-1)] = data[j+i*cols];
                else
                    nd[j-1+i*(cols-1)] = data[j+i*cols];
            }
        }
        data = nd;
        cols--;
    }

    void dump() {
        for(int i=0; i<rows; ++i) {
            printf("constraint: [");
            for(int k=0; k<cols; ++k)
                printf("%8.2f%s",data[k+i*cols],k==cols-1?"]\n":",");
        }
    }
    int  length() {return rows*cols;}
    float *data;
    int rows;
    int cols;
    float &operator()(int r, int c) {
        assert(r<rows);
        assert(r>=0);
        assert(c<cols);
        assert(c>=0);
        return data[c+r*cols];
    }
};

struct LpResults
{
    Matrix solution;
    float zmin;
    bool is_bounded;
    bool is_solved;
    Matrix basis;

};

struct SimplexResults
{
    Matrix table;
    Matrix basis;
    bool is_bounded;
};

#define Inf (1.0/0.0)

Matrix pivot(Matrix H, int r, int c)
{
    int n = H.rows;
    int m = H.cols;
    float piv = H(r,c);
    printf("Pivot pt = %f\n", piv);
    if(piv == 0) {
        printf("Warning: Singluar Matrix...\n");
    } else {
        //Normalize the row
        for(int i=0; i<H.cols; ++i)
            H(r, i) /= piv;
        for(int i=0; i<n; ++i) {
            if(i != r) {
                float gain = H(i,c);
                for(int j=0; j<m; ++j) {
                    H(i,j) -= gain*H(r,j);
                }
            }
        }
    }
    return H;
}

SimplexResults simplex(Matrix H, Matrix basis, Matrix index, int s0)
{
    SimplexResults results{{H.rows,H.cols},{basis.rows,basis.cols}, 0};

    int n1 = H.rows;
    int n2 = H.cols;
    bool solved = false;
    while(!solved) {
        printf(".");
        //value index
        float min_obj = Inf;
        int   min_col  = -1;
        for(int i=0; i<n2-1; ++i) {
            if(H(n1-1,i) < min_obj) {
                min_obj = H(n1-1,i);
                min_col = i;
            }
        }
        assert(min_col != -1);
        if(min_obj >=0) {
            results.is_bounded = true;
            solved = true;
        } else {
            float max_con = -Inf;
            int   max_row = -1;
            for(int i=0; i<n1-s0; ++i) {
                if(H(i, min_col) > max_con) {
                    max_con = H(i, min_col);
                    max_row = i;
                }
            }
            assert(max_row != -1);
            if(max_con <= 0) {
                printf("Unbounded Equation..\n");
                solved = true;
                results.is_bounded = false;
            } else {
                float min_unknown = Inf;
                int   min_id      = -1;
                for(int i=0; i<n1-s0; ++i) {
                    float v = 0;
                    if(H(i,min_col) > 0)
                        v = H(i,n2-1)/H(i,min_col);
                    else
                        v = Inf;
                    if(v<min_unknown) {
                        min_unknown = v;
                        min_id = i;
                    }
                }
                printf("performing pivot[%d,%d]...\n",min_id,min_col);
                assert(min_id != -1);
                basis(min_id,0) = index(min_col,0);
                H = pivot(H, min_id, min_col);
                H.dump();
            }
        }
    }
    results.table = H;
    results.basis = basis;
    return results;
}

#define epsilon (1e-10)

//We start with a matrix of the form
//[A  b]
//[c  0]
//[d -w]
LpResults linear_program(Matrix constraint_eqn)
{
    LpResults results;
    auto H = constraint_eqn;
    const int m = H.rows-2;//number of linear constraints
    const int n = H.cols-1;//number of variables (including slack)

    //Phase One
    Matrix index(n,1);
    for(int i=0;i<n; ++i)
        index(i,0) = i;
    Matrix basis(m,1);
    for(int i=0;i<m;++i)
        basis(i,0) = n+i;
    SimplexResults sr1 = simplex(H, basis, index, 2);
    H = sr1.table;
    basis = sr1.basis;
    if(H(m+1, n) < -epsilon) {
        results.is_solved = false;
        printf("Cannot Solve Linear Program...\n");
        //return;
    }
    results.is_solved = true;
    int j=0;
    for(int i=1; i<n; ++i) {
        //Delete the column for some reason?
        if(H(m+1,j) > epsilon && false){
            printf("Deleting Column<%d>***********************\n",j);
            H.delCol(j);
            index.delRow(j);
            j--;
        }
        j++;
    }
    H.delRow(H.rows-1);
    printf("Phase 2\n");
    H.dump();
    if(index.length() == 0)
    {
        printf("Trivial Solution..\n");
    }
    printf("Simplex some more...\n");
    SimplexResults sr2 = simplex(H, basis, index, 1);
    printf("Simplex done...\n");
    H     = sr2.table;
    basis = sr2.basis;
    if(sr2.is_bounded)
    {
        Matrix optx = Matrix(n+m,1);
        int n1=H.rows;
        int n2=H.cols;
        printf("trying to print results...\n");
        printf("Basis.length = %d\n", basis.length());
        printf("H.size=[%d,%d]\n", H.rows, H.cols);
        for(int i=0; i<m; ++i) {
            printf("i=%d\n",i);
            fflush(stdout);
            printf("%d => %f\n", (int)basis(i,0), H(i,n2-1));
            optx(basis(i,0),0)=H(i,n2-1);
        }
        optx.dump();
        results.solution = optx;
        results.zmin = -H(n1-1, n2-1);
    } else {
        float optx = 0;
        float zmin = -Inf;
    }
    return results;
}
