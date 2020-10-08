#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include <vector>
const int MAXN = 505;
//* 此类为参考刘汝佳蓝书修改本人代码而成,不会添加详细注释
//* 若需详细注释请阅读非OOP版舞蹈链
//! 此代码必然会跑得很慢,建议搭配氧气使用
class DLX
{
private:
    struct Node{ int l,r,u,d,row,col; };
    vector<Node> dcl;
    vector<int> s,ans;

    void remove(int col)
    {
        dcl[dcl[col].r].l = dcl[col].l;
        dcl[dcl[col].l].r = dcl[col].r;
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)
        {
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
            {
                dcl[dcl[j].d].u = dcl[j].u;
                dcl[dcl[j].u].d = dcl[j].d;
                s[dcl[j].col]--;
            }
        }
    }
    void restore(int col)
    {
        dcl[dcl[col].r].l = col;
        dcl[dcl[col].l].r = col;
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)
        {
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
            {
                dcl[dcl[j].d].u = j;
                dcl[dcl[j].u].d = j;
                s[dcl[j].col]++;
            }
        }
    }
    bool dance()
    {
        int col = dcl[0].r;
        if(!col)
            return true;
        for(int i=col;i;i=dcl[i].r)
            if(s[i]<s[col]) col = i;
        remove(col);
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)
        {
            ans.push_back(dcl[i].row);
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
                remove(dcl[j].col);
            if(dance()) return true;
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
                restore(dcl[j].col);
            ans.pop_back();
        }
        restore(col);
        return false;
    }

public:
    DLX(int col)
    {
        s.resize(col+5);       //动态开出空间
        for(int i=0;i<=col;i++)
            dcl.push_back( {i-1,i+1,i,i,0,0} );
        dcl[0].l = col;
        dcl[col].r = 0;
    }
    void addRow(int r,vector<int>& col)     //为十字链表增加一行,r为行编号,col中存放值为1的列的编号
    {
        int first = dcl.size();
        for(int c : col)
        {
            int tmp = dcl.size();
            dcl.push_back( {tmp-1,tmp+1,dcl[c].u,c,r,c} );
            dcl[c].u = dcl[dcl[c].u].d = tmp;
            s[c]++;
        }
        dcl[dcl.size()-1].r = first;
        dcl[first].l = dcl.size()-1;
    }
    auto& solve() { return dance(),ans; }   //若无解则ans必为空
};
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
    DLX solver(m);          //实例化
    for(int i=1;i<=n;i++)
    {
        vector<int> v;
        for(int j=1;j<=m;j++)
        {
            int tmp;
            cin>>tmp;
            if(tmp) v.push_back(j);     //注意存放的是列编号
        }
        solver.addRow(i,v);
    }
    auto ans = solver.solve();
    if(ans.empty()) cout<<"No Solution!"<<endl;
    else for(auto i : ans) cout<<i<<' ';
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}