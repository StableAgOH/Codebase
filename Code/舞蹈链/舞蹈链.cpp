#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cmath>
#include <ctime>
using namespace std;
//==========================================
const int MAXN = 505;
struct Node
{
    int l,r,u,d,row,col;        //舞蹈链结点中的内容
}dcl[MAXN*MAXN];
int cnt;                        //内存池计数器
int s[MAXN];                    //用于存储某列有多少个存在的结点
stack<int> ans;                 //答案栈
void init(int m)                //建立Head结点与列首结点
{
    for(int i=0;i<=m;i++)       //算上Head一共m+1个,其中dcl[0]代表Head
    {
        dcl[i].u = dcl[i].d = i;    //一开始是没有上下结点的,循环链表所以指向自己
        dcl[i].l = i-1, dcl[i].r = i+1;  //但是有左右结点
    }
    dcl[m].r = 0;               //循环链表
    dcl[0].l = m;
    cnt = m+1;                  //注意内存池计数器要变
}
void build(int n,int m)
{
    init(m);
    for(int i=1;i<=n;i++)
    {
        int first = cnt;        //记录行首结点编号
        for(int j=1;j<=m;j++)
        {
            int tmp;            //模板题目,直接读入数据
            cin>>tmp;
            if(tmp)
            {
                dcl[cnt].l = cnt-1;
                dcl[cnt].r = cnt+1;
                dcl[cnt].u = dcl[j].u;
                dcl[cnt].d = j;           //循环链表
                dcl[cnt].row = i;
                dcl[cnt].col = j;
                dcl[dcl[j].u].d = cnt;    //dcl[j]即为列首元素
                dcl[j].u = cnt;           //循环链表
                s[j]++, cnt++;
            }
        }
        dcl[cnt-1].r = first;     //循环链表
        dcl[first].l = cnt-1;
    }
}
void remove(int c)  //删除c列以及c列中结点所在行中的结点所在的列
{
    dcl[dcl[c].r].l = dcl[c].l;               //先把列首元素的关系断开
    dcl[dcl[c].l].r = dcl[c].r;
    for(int i=dcl[c].d;i!=c;i=dcl[i].d)     //遍历c列
    {
        for(int j=dcl[i].r;j!=i;j=dcl[j].r) //遍历c列中结点所在行中的结点
        {
            dcl[dcl[j].d].u = dcl[j].u;       //断掉上下关系,注意没有断左右关系
            dcl[dcl[j].u].d = dcl[j].d;
            s[dcl[j].col]--;
        }
    }
}
void restore(int c) //恢复c列以及c列中结点所在行中的结点所在的列,把remove操作反着进行一次即可
{
    dcl[dcl[c].r].l = c;
    dcl[dcl[c].l].r = c;
    for(int i=dcl[c].d;i!=c;i=dcl[i].d)
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
    if(!dcl[0].r)     //如果十字链表中只剩下Head结点了
        return true;
    int c = dcl[0].r;
    for(int i=c;i;i=dcl[i].r)
        if(s[i]<s[c]) c = i;      //找到结点数量最小的列,用来防止TLE
    remove(c);
    for(int i=dcl[c].d;i!=c;i=dcl[i].d)
    {
        ans.push(dcl[i].row);
        for(int j=dcl[i].r;j!=i;j=dcl[j].r)
            remove(dcl[j].col);
        if(dance()) return true;
        for(int j=dcl[i].r;j!=i;j=dcl[j].r)
            restore(dcl[j].col);
        ans.pop();
    }
    restore(c);
    return false;
}
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
    build(n,m);
    if(!dance()) cout<<"No Solution!"<<endl;
    else while(ans.size())
    {
        cout<<ans.top()<<' ';
        ans.pop();
    }
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}