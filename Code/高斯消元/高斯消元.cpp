#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include <iomanip>
#include <cstring>
#include <cmath>
const int MAXN = 105;
const double EPS = 1e-8;
struct Matrix
{
    int r,c;
    double ele[MAXN][MAXN];
    Matrix(int r,int c):r(r),c(c) { memset(ele,0,sizeof(ele)); }
    double &operator()(int r,int c) { return ele[r][c]; }
};
istream &operator>>(istream &is, Matrix &mat)
{
    for (int i=0;i<mat.r;i++)
        for (int j=0;j<mat.c;j++)
            is>>mat(i,j);
    return is;
}
ostream &operator<<(ostream &os, Matrix &mat)
{
    for(int i=0;i<mat.r;i++)
    {
        for(int j=0;j<mat.c;j++)
            os<<fixed<<setprecision(2)<<mat(i,j)<<' ';
        os<<endl;
    }
    return os;
}
bool gauss(Matrix &mat)
{
    for(int i=0;i<mat.r;i++)    //遍历行
    {
        int p = i;  //当前系数
        for(int j=i+1;j<mat.r;j++)  //遍历每行的当前位置
            if(abs(mat(p,i))<abs(mat(j,i)))
                p = j;
        if(abs(mat(p,i))<EPS) return false;
        if(p!=i) for(int j=0;j<mat.c;j++)
            swap(mat(i,j),mat(p,j));
        for(int j=i+1;j<mat.c;j++)
            mat(i,j)/=mat(i,i);
        for(int j=0;j<mat.r;j++)
        {
            if(i==j) continue;
            for(int k=i+1;k<mat.c;k++)
                mat(j,k)-=mat(j,i)*mat(i,k);
        }
    }
    return true;
}
void print(Matrix &mat)
{
    for(int i=0;i<mat.r;i++)
        cout<<fixed<<setprecision(2)<<mat(i,mat.c-1)<<endl;
}
int main(int argc, char const *argv[])
{
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int n;
    cin>>n;
    static Matrix mat(n,n+1);
    cin>>mat;
    if(gauss(mat)) print(mat);
    else cout<<"No Solution"<<endl;
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}