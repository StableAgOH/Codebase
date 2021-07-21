#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include <cmath>
const int maxn = 1e5 + 5;
typedef pair<int, int> pii;     // first元素存放救济粮数量，second存放该救济粮类型号
struct Node
{
    int l,r;
    pii val;
}sgt[70*maxn];                  //? 70 = 4*maxm*log2(maxn)
int cnt, root[maxn];
pii& max(pii& x, pii& y)        // 因为要取类型号较小的所以无法直接使用max比较
{
    if(x.first>y.first) return x;
    else if(x.first==y.first) return x.second<y.second?x:y;   // 若数量相等取类型号比较小的
    else return y;
}
inline void pushup(int k) { sgt[k].val=max(sgt[sgt[k].l].val, sgt[sgt[k].r].val); }
// 以下为链式前向星
struct E { int v,next; } Edge[maxn<<1];
int tot,Head[maxn];
inline void AddEdge(int u,int v)
{
    Edge[tot]=(E){v,Head[u]};
    Head[u]=tot++;
}
// 以下为树剖求LCA
int dep[maxn], fa[maxn], siz[maxn], son[maxn];
void dfs1(int u,int f)
{
    fa[u]=f, siz[u]=1, dep[u]=dep[f]+1;
    int maxsize = -1;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].v;
        if(v==f) continue;
        dfs1(v, u);
        siz[u] += siz[v];
        if(siz[v]>maxsize)
        {
            son[u]=v;
            maxsize = siz[v];
        }
    }
}
int top[maxn];
void dfs2(int u,int t)
{
    top[u]=t;
    if(son[u]) dfs2(son[u], t);
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].v;
        if(v==fa[u]||v==son[u]) continue;
        dfs2(v, v);
    }
}
int lca(int x,int y)
{
    while(top[x]!=top[y])
    {
        if(dep[top[x]]<dep[top[y]])
            swap(x,y);
        x = fa[top[x]];
    }
    return dep[x]<dep[y]?x:y;
}
void modify(int l,int r,int &k,int p,int x)     // 单点修改，空间复杂度O(logn)
{
    if(!k) k=++cnt;
    if(l==r) 
    {
        sgt[k].val.first += x;      // 到叶子结点时使数量加x
        sgt[k].val.second = p;      // 同时捆绑保存住救济粮类型号
        return;
    }
    int m = (l+r)>>1;
    if(p<=m) modify(l, m, sgt[k].l, p, x);
    else modify(m+1, r, sgt[k].r, p, x);
    pushup(k);
}
void merge(int &x,int y,int l=1,int r=maxn)     // 此处的maxn其实是maxk，l与r用于方便判断是否到了叶子结点
{
    if(!(x&&y))
        x|=y;
    else if(l==r) 
        sgt[x].val.first += sgt[y].val.first;   // 到叶子结点时处理有可能发生的同类型救济粮之间的数量叠加
    else
    {
        int m = (l+r)>>1;
        merge(sgt[x].l, sgt[y].l, l, m);
        merge(sgt[x].r, sgt[y].r, m+1, r);
        pushup(x);
    }
}
int ans[maxn];
void dfs(int u)     // dfs处理树上差分
{
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].v;
        if(v==fa[u]) continue;
        dfs(v);     // 头递归
        merge(root[u], root[v]);    // 合并上所有的儿子结点
    }
    if(sgt[root[u]].val.first)      // 若数量不为0，那么second即为u结点数量最多的救济粮
        ans[u]=sgt[root[u]].val.second;
}
#include <cstring>
signed main(signed argc, char const *argv[])
{
    clock_t c1 = clock();
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    //======================================
    memset(Head, -1, sizeof(Head));
    int n,m;
    cin>>n>>m;
    for(int i=1;i<n;i++)
    {
        int u,v;
        cin>>u>>v;
        AddEdge(u,v);
        AddEdge(v,u);
    }
    dfs1(1, 0);
    dfs2(1, 0);
    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
        // 以下为树上差分基操
        modify(1, maxn, root[x], z, 1);     // x结点加上z救济粮标记
        modify(1, maxn, root[y], z, 1);     // y结点加上z救济粮标记
        modify(1, maxn, root[lca(x,y)], z, -1);     // lca(x,y)结点加上-z救济粮标记
        modify(1, maxn, root[fa[lca(x,y)]], z, -1); // fa[lca(x,y)]结点加上-z救济粮标记
    }
    dfs(1);     // 差分统计
    for(int i=1;i<=n;i++) cout<<ans[i]<<endl;
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}