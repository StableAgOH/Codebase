#include <iostream>
#include <ctime>
#include <cstdio>
#include <cctype>
namespace FastIO
{
char buf[1 << 21], buf2[1 << 21], a[20], *p1 = buf, *p2 = buf, hh1 = '\n', hh2 = ' ';
int p, p3 = -1;
void read() {}
void print() { buf2[p3] = hh1; }
inline int getc()
{
    return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 21, stdin), p1 == p2) ? EOF : *p1++;
}
inline void flush()
{
    fwrite(buf2, 1, p3 + 1, stdout), p3 = -1;
}
template <typename T, typename... T2>
inline void read(T &x, T2 &... oth)
{
    int f = 0;
    x = 0;
    char ch = getc();
    while (!isdigit(ch))
    {
        if (ch == '-')
            f = 1;
        ch = getc();
    }
    while (isdigit(ch))
    {
        x = x * 10 + ch - 48;
        ch = getc();
    }
    x = f ? -x : x;
    read(oth...);
}
template <typename T, typename... T2>
inline void print(T x, T2... oth)
{
    if (p3 > 1 << 20)
        flush();
    if (x < 0)
        buf2[++p3] = 45, x = -x;
    do
    {
        a[++p] = x % 10 + 48;
    } while (x /= 10);
    do
    {
        buf2[++p3] = a[p];
    } while (--p);
    buf2[++p3] = hh2;
    print(oth...);
}
} // namespace FastIO
#define read FastIO::read
#define print FastIO::print
//======================================
#include <cstring>
const int maxn = 1e5+5;
const int INF = 0x3f3f3f3f;     //极大值，-INF就是极小值
//! 这里注意极小值不能使用INT_MIN,因为修改操作有负数，会导致小爆INT
//TODO 链式前向星
struct E
{
    int to,next;
}Edge[maxn<<1];
int tot,Head[maxn];
inline void AddEdge(int u,int v)
{
    Edge[tot].to=v;
    Edge[tot].next=Head[u];
    Head[u]=tot++;
}
//TODO 为了学这个又写了个简化版的矩阵模板，方便快捷好写好用，可以学习一下，Github上有
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
Matrix<2,2> ident,g[maxn];  //ident为单位矩阵，在主函数中初始化，g为矩阵池，存放x结点的矩阵是什么
int val[maxn],f[2][maxn];   //val存放各结点权值，f用于初始化矩阵池g
//TODO 树链剖分第一遍dfs，与普通树链剖分无区别
int fa[maxn],dep[maxn],siz[maxn],son[maxn];
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
//TODO 树链剖分第二遍dfs，在dfs过程中初始化矩阵池
int tim,dfn[maxn],nfd[maxn],top[maxn],end[maxn];
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
    //? g处理好后即可处理g，f再用于处理其父亲结点的g
    f[0][u]=g[u](0,0)+std::max(f[0][son[u]],f[1][son[u]]);
    f[1][u]=g[u](1,0)+f[0][son[u]];
}
//TODO 线段树
struct Node
{
    int l,r;
    Matrix<2,2> val;
}sgt[maxn<<2];
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
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    memset(Head,-1,sizeof(Head));   //链式前向星-1写法
    ident(1,0)=ident(0,1)=-INF;     //单位矩阵赋值
    int n,m;
    read(n,m);
    for(int i=1;i<=n;i++) read(val[i]);
    for(int i=1;i<n;i++)
    {
        int u,v;
        read(u,v);
        AddEdge(u,v),AddEdge(v,u);
    }
    dfs1(1,0);                      //这里注意(1,0)
    dfs2(1,1);
    build(1,n);
    while(m--)
    {
        int x,z;
        read(x,z);
        modifyt(x,z);
        auto ans = queryt(1);               //查询根结点矩阵，结果为一个2×1矩阵
        print(std::max(ans(0,0),ans(1,0))); //查出的矩阵中较大的元素即为答案
    }
    //======================================
    FastIO::flush();
    std::cerr << "Time:" << clock() - c1 << "ms" << std::endl;
    return 0;
}
/*
 * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
 * │Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │P/S│S L│P/B│  ┌┐    ┌┐    ┌┐
 * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘  └┘    └┘    └┘
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
 * │~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│) 0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │Num│ / │ * │ - │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
 * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│ | \ │ │Del│End│PDn│ │ 7 │ 8 │ 9 │   │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
 * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  │               │ 4 │ 5 │ 6 │   │
 * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
 * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│  Shift   │     │ ↑ │     │ 1 │ 2 │ 3 │   │
 * ├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤ E││
 * │ Ctrl│ Win│ Alt│         Space         │ Alt│ Win│Menu│Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │←─┘│
 * └─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
 */