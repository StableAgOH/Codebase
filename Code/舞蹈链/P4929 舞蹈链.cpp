#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include "DLX.h"
int main(int argc, char const *argv[])
{
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int n,m;
    cin>>n>>m;
    DLX solver(m);
    for(int i=1;i<=n;i++)
    {
        vector<int> v;
        for(int j=1;j<=m;j++)
        {
            int tmp;
            cin>>tmp;
            if(tmp) v.push_back(j);
        }
        solver.addRow(i,v);
    }
    if(!solver.solve([](vector<int> v){
        for(auto i : v) cout<<i<<' ';
    })) cout<<"No Solution!"<<endl;
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}