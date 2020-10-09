#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include <vector>
#include <tuple>
class DLX
{
private:
    struct Node{ int l,r,u,d,row,col; };
    std::vector<Node> dcl;
    std::vector<int> s,ans;
    bool ms;                                        //是否多解
    void (*process)(std::vector<int>&);             //在得到一个解后对答案进行处理

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
        {
            process(ans);                           //得到了一个解
            return true;
        }
        for(int i=col;i;i=dcl[i].r)
            if(s[i]<s[col]) col = i;
        remove(col);
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)
        {
            ans.push_back(dcl[i].row);
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
                remove(dcl[j].col);
            bool flag = dance();
            if(flag&&!ms) return true;              //若只需要一个解则return
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
                restore(dcl[j].col);
            ans.pop_back();
        }
        restore(col);
        return false;
    }

public:
    DLX(int col,bool hasMultipleSolutions=false)    //传入列数及是否多解
    {
        ms = hasMultipleSolutions;
        s.resize(col+5);                            //动态开出空间
        for(int i=0;i<=col;i++)
            dcl.push_back( {i-1,i+1,i,i,0,0} );
        dcl[0].l = col;
        dcl[col].r = 0;
    }
    void addRow(int row,std::vector<int>& col)      //为十字链表增加一行,row为行编号,col中存放值为1的列的编号
    {
        int first = dcl.size();
        for(int c : col)
        {
            int tmp = dcl.size();
            dcl.push_back( {tmp-1,tmp+1,dcl[c].u,c,row,c} );
            dcl[c].u = dcl[dcl[c].u].d = tmp;
            s[c]++;
        }
        dcl[dcl.size()-1].r = first;
        dcl[first].l = dcl.size()-1;
    }
    bool solve(void (*fun)(std::vector<int>&))      //传入一个用于处理每个解的答案的函数
    {
        process = fun;
        return dance();
    }
};
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
inline void transandprint(vector<int>& v)
{
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
    solver.solve(transandprint);
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}