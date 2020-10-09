#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include <tuple>
#include "DLX.h"
int sdk[11][11];
enum Task { Grid,Row,Col,Sub };
inline int encode(int x,int r,int c)
{
    return x*9*9+(r-1)*9+c;
}
inline auto decode(int n)
{
    n--;
    int x,r,c;
    c = n%9+1; n/=9;
    r = n%9+1; n/=9;
    x = n;
    return make_tuple(x,r,c);
}
int main(int argc, char const *argv[])
{
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    DLX solver(4*9*9);
    for(int i=1;i<=9;i++)
    {
        for(int j=1;j<=9;j++)
        {
            int tmp;
            cin>>tmp;
            for(int k=1;k<=9;k++)
            {
                vector<int> v;
                if(tmp&&tmp!=k) continue;
                v.push_back(encode(Grid,i,j));
                v.push_back(encode(Row,i,k));
                v.push_back(encode(Col,j,k));
                int r = (i-1)/3;
                int c = (j-1)/3+1;
                v.push_back(encode(Sub,r*3+c,k));
                solver.addRow(encode(k-1,i,j),v);
            }
        }
    }
    solver.solve([](vector<int> v) {
        for(auto i : v)
        {
            int x,r,c;
            tie(x,r,c) = decode(i);
            sdk[r][c] = x+1;
        }
        for(int i=1;i<=9;i++)
        {
            for(int j=1;j<=9;j++)
                cout<<sdk[i][j]<<' ';
            cout<<endl;
        }
    });
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}