#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstring>
const int MAXN = 1e5+5;
const int INF = 0x3f3f3f3f;     //极大值，-INF就是极小值
//! 这里注意极小值不能使用INT_MIN,因为修改操作有负数，会导致小爆INT
//链式前向星存图
struct E { int to,next; } Edge[MAXN<<1];
int tot,Head[MAXN];
inline void AddEdge(int u,int v)
{
    Edge[tot]=(E){v, Head[u]};
    Head[u]=tot++;
}
//! 这个矩阵模板写错了不要学
template <int row,int col>
struct Matrix
{
    int r,c;
    int ele[row][col];          //元素
    Matrix():r(row),c(col) {}
    int& operator()(int a,int b) { return ele[a][b]; }  //用括号运算符获取矩阵中元素
};
template <int m,int n,int p>
auto operator*(Matrix<m,n> m1,Matrix<n,p> m2)   //如果不用C++14就要把前面的auto换成Matrix<m,p>
{
    Matrix<m,p> ret;            //用于存储结果
    //? 因为新的矩乘法则，初始化为极小值
    memset(ret.ele,0xcf,sizeof(ret.ele));
    for(int i=0;i<m;i++)        //同样可以使用ikj优化
        for(int k=0;k<n;k++)
            for(int j=0;j<p;j++)
                ret(i,j)=std::max(ret(i,j),m1(i,k)+m2(k,j));
    return ret;
}
Matrix<2,2> ident,g[MAXN];  //ident为单位矩阵，在主函数中初始化，g为矩阵池，存放x结点的矩阵是什么
int val[MAXN],f[2][MAXN];   //val存放各结点权值，f用于初始化矩阵池g
//树链剖分第一遍dfs，与普通树链剖分无区别
int fa[MAXN],dep[MAXN],siz[MAXN],son[MAXN];
void dfs1(int u,int f)
{
    fa[u]=f,siz[u]=1,dep[u]=dep[f]+1;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==f) continue;
        dfs1(v,u);
        siz[u]+=siz[v];
        if(siz[v]>siz[son[u]])
            son[u]=v;
    }
}
//树链剖分第二遍dfs，在dfs过程中初始化矩阵池
int tim,dfn[MAXN],nfd[MAXN],top[MAXN],end[MAXN];
void dfs2(int u,int t)
{
    top[u]=t,dfn[u]=++tim,nfd[tim]=u;   //nfd[x]存放dfn为x的是哪个结点
    if(!son[u])                         //如果没有重儿子（没有儿子）即为叶子结点
    {
        f[1][u]=val[u];                 //叶子结点的f
        g[u]=ident;                     //叶子结点的矩阵为单位矩阵
        end[u]=u;                       //叶子结点所在重链尾端即为此叶子结点
        return;
    }
    g[u](1,0)=val[u],g[u](1,1)=-INF;    //初始化矩阵，左下角放上点权，右下角一直是INF
    dfs2(son[u],t);
    end[u]=end[son[u]];         //整条重链的end值都等于最下方叶结点，这样一层一层传上来
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==fa[u]||v==son[u]) continue;
        dfs2(v,v);                      //这都是轻儿子
        g[u](0,0)=g[u](0,1)+=std::max(f[0][v],f[1][v]);//每个轻儿子处理好后，将值加到g(i,0)和g(i,1)里
        g[u](1,0)+=f[0][v];
    }
    //? g处理好后即可处理f，f再用于处理其父亲结点的g
    f[0][u]=g[u](0,0)+std::max(f[0][son[u]],f[1][son[u]]);
    f[1][u]=g[u](1,0)+f[0][son[u]];
}
//线段树
struct Node
{
    int l,r;
    Matrix<2,2> val;
}sgt[MAXN<<2];
#define ls(x) (x<<1)
#define rs(x) (x<<1|1)
#define pushup(x) sgt[x].val=sgt[ls(x)].val*sgt[rs(x)].val
void build(int l,int r,int k=1)
{
    sgt[k].l=l,sgt[k].r=r;
    if(l==r)
    {
        sgt[k].val=g[nfd[l]];   //在矩阵池里获取应该赋值的矩阵
        return;
    }
    int m = (l+r)>>1;
    build(l,m,ls(k));
    build(m+1,r,rs(k));
    pushup(k);
}
auto query(int x,int y,int k=1) //区间查询乘积
{
    int l=sgt[k].l,r=sgt[k].r;
    if(x<=l&&y>=r) return sgt[k].val;
    int m = (l+r)>>1;
    Matrix<2,2> ret = ident;    //应初始化为单位矩阵
    if(x<=m) ret = query(x,y,ls(k));
    if(y>m) ret = ret * query(x,y,rs(k));   //注意右乘
    return ret;
}
void modify(int x,int p,int k=1) //单点修改x结点的矩阵为矩阵池中p位置上的矩阵
{
    int l=sgt[k].l,r=sgt[k].r;
    if(l==r)
    {
        sgt[k].val=g[p];
        return;
    }
    int m = (l+r)>>1;
    if(x<=m) modify(x,p,ls(k));
    else modify(x,p,rs(k));
    pushup(k);
}
auto queryt(int x)          //查询x结点的f(x,0),f(x,1)矩阵
{
    Matrix<2,1> tmp;
    tmp(0,0)=0,tmp(1,0)=val[end[x]];        //初始矩阵，PPT里有说
    return query(dfn[x],dfn[end[x]]) * tmp; //注意左乘
}
void modifyt(int x,int z)   //单点修改x结点的权值为z
{
    Matrix<2,1> od,nw;
    g[x](1,0)+=z-val[x];    //相当于先减val[x]再加z，加法交换律
    od = queryt(top[x]);    //修改之前先查询一个旧的矩阵
    val[x]=z;               //然后修改，因为queryt函数里初始矩阵是用val[x]算出来的
    while(x)                //只要x没跳到头，这样写的话主函数里dfs1调用时第二个参数就必须为0
    {
        modify(dfn[x],x);       //先修改
        nw = queryt(top[x]);    //再查一个新的矩阵出来
        x = fa[top[x]];
        //? 在矩阵池里修改，用于下次线段树上修改
        g[x](0,0)=g[x](0,1)+=std::max(nw(0,0),nw(1,0))-std::max(od(0,0),od(1,0));
        g[x](1,0)+=nw(0,0)-od(0,0);
        od = queryt(top[x]);    //下次修改之前先查一个老的出来
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
    memset(Head,-1,sizeof(Head));   //链式前向星-1写法
    ident(1,0)=ident(0,1)=-INF;     //单位矩阵赋值
    int n,m;
    std::cin>>n>>m;
    for(int i=1;i<=n;i++) std::cin>>val[i];
    for(int i=1;i<n;i++)
    {
        int u,v;
        std::cin>>u>>v;
        AddEdge(u,v),AddEdge(v,u);
    }
    dfs1(1,0);                      //这里注意(1,0)
    dfs2(1,1);
    build(1,n);
    while(m--)
    {
        int x,z;
        std::cin>>x>>z;
        modifyt(x,z);
        auto ans = queryt(1);               //查询根结点矩阵，结果为一个2×1矩阵
        std::cout<< std::max(ans(0,0),ans(1,0)) << std::endl; //查出的矩阵中较大的元素即为答案
    }
    //======================================
    std::cerr << "Time:" << clock() - c1 << "ms" << std::endl;
    return 0;
}
