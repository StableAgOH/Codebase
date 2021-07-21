#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
const int maxn = 2e5 + 5;
typedef long long ll;
struct Node
{
    int l,r;
    ll val;
}sgt[maxn*40];      //? 40 = 2*maxm*log2(maxn)
int cnt,root[maxn];
inline void pushup(int k) { sgt[k].val = sgt[sgt[k].l].val + sgt[sgt[k].r].val; }
void modify(int l,int r,int &k,int p,int x)     // 单点修改:p位置的值加上x，空间复杂度O(logn)
{
    if(!k) k=++cnt;     // 如果到了NIL结点就新建一个
    sgt[k].val += x;    // 单点修改的加法直接一条线上全部加上x即可
    if(l==r) return;
    int m = (l+r)>>1;
    if(p<=m) modify(l, m, sgt[k].l, p, x);      // 在左子树
    else modify(m+1, r, sgt[k].r, p, x);        // 在右子树
}
void merge(int &x,int y)        // 把y结点的内容合并到x结点上，此写法不消耗空间
{
    if(!(x&&y)) x|=y;           // 如果二者有NULL结点
    else 
    {
        sgt[x].val += sgt[y].val;   // 维护加法，直接加就是了
        merge(sgt[x].l, sgt[y].l);  // 递归合并两结点的左子树
        merge(sgt[x].r, sgt[y].r);  // 递归合并两结点的右子树
    }
}
int split(int l,int r,int &k,int x,int y)   // 从k中分离出[x,y]区间并返回新结点编号，空间复杂度O(2logn)
{
    int n = ++cnt;
    if(x<=l&&y>=r)      // 如果k结点维护的区间在[x,y]中
    {
        sgt[n] = sgt[k];    // 直接拿过来便是
        k = 0;              // 置为NULL，断掉联系
    }
    else 
    {
        int m = (l+r)>>1;
        if(x<=m) sgt[n].l = split(l, m, sgt[k].l, x, y);        // 若左子树中有区间信息
        if(y>m)  sgt[n].r = split(m+1, r, sgt[k].r, x, y);      // 若右子树中有区间信息
        pushup(k);
        pushup(n);      // 更改后记得更新值
    }
    return n;
}
ll query(int l,int r,int k,int x,int y)     // 区间查询
{
    if(x<=l&&y>=r) return sgt[k].val;
    int m = (l+r)>>1;
    ll sum = 0;
    if(x<=m) sum += query(l,m,sgt[k].l,x,y);
    if(y>m)  sum += query(m+1,r,sgt[k].r,x,y);
    return sum;
}
ll query(int l,int r,int k,int kth)         // 单点查询
{
    if(l==r) return l;
    int m = (l+r)>>1;
    if(kth<=sgt[sgt[k].l].val) return query(l,m,sgt[k].l,kth);
    else return query(m+1,r,sgt[k].r,kth-sgt[sgt[k].l].val);
}
signed main(signed argc, char const *argv[])
{
    clock_t c1 = clock();
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    //======================================
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=n;i++)
    {
        int x;
        cin>>x;
        modify(1,n,root[1],i,x);            // 初始状态下值都为0，所以加x即为置为x
    }
    int last = 1;
    while(m--)
    {
        int opt,x,y,z;
        cin>>opt>>x>>y;
        switch(opt)
        {
        case 0:
            cin>>z;
            root[++last] = split(1, n, root[x], y, z);
            break;
        case 1:
            merge(root[x],root[y]);
            break;
        case 2:
            cin>>z;
            modify(1,n,root[x],z,y);
            break;
        case 3:
            cin>>z;
            cout<<query(1,n,root[x],y,z)<<endl;
            break;
        case 4:
            if(y>sgt[root[x]].val) cout<<-1<<endl;      // 若只有4个元素却来查询第5大元素（例如），那么结果即为-1
            else cout<<query(1,n,root[x],y)<<endl;
            break;
        }
    }
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}