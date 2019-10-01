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
using FastIO::print;
using FastIO::read;
//======================================
const int maxn = 1e4+5;
const int maxk = 1e7+5;
struct E
{
    int to,w,next;
}Edge[maxn<<1];
int tot,Head[maxn];
inline void AddEdge(int u,int v,int w)
{
    Edge[tot]=(E){v,w,Head[u]};
    Head[u]=tot++;
}
//? rt记录重心，sum记录当前树大小，cnt是计数器
int n,m,rt,sum,cnt;
//? tmp记录算出的距离，siz记录子树大小，dis[i]为rt与i之间的距离
//? maxp用于找重心，q用于记录所有询问
int tmp[maxn],siz[maxn],dis[maxn],maxp[maxn],q[105];
//? judge[i]记录在之前子树中距离i是否存在，ans记录第k个询问是否存在，vis记录被删除的结点
bool judge[maxk],ans[105],vis[maxn];
//TODO 找重心
void getrt(int u,int f)
{
    siz[u]=1,maxp[u]=0; //maxp初始化为最小值
    //遍历所有儿子，用maxp保存最大大小的儿子的大小
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==f||vis[v]) continue;  //被删掉的也不要算
        getrt(v,u);
        siz[u]+=siz[v];
        if(siz[v]>maxp[u]) maxp[u]=siz[v];  //更新maxp
    }
    maxp[u]=std::max(maxp[u],sum-siz[u]);   //考虑u的祖先结点
    if(maxp[u]<maxp[rt]) rt=u;      //更新重心（最大子树大小最小）
}
//TODO 计算各结点与根结点之间的距离并全部记录在tmp里
void getdis(int u,int f)
{
    tmp[cnt++]=dis[u];
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==f||vis[v]) continue;
        dis[v]=dis[u]+Edge[i].w;
        getdis(v,u);
    }
}
#include <queue>
//TODO 处理经过根结点的路径
//! 注意judge数组要存放之前子树里存在的路径长度，排除折返路径的可能
void solve(int u)
{
    static std::queue<int> que;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(vis[v]) continue;
        cnt=0;              //注意置零计数器
        dis[v]=Edge[i].w;
        getdis(v,u);        //把距离都处理出来
        for(int j=0;j<cnt;j++)      //遍历所有距离
            for(int k=0;k<m;k++)    //遍历所有询问
                if(q[k]>=tmp[j])    //如果询问大于单条路径长度，那就有可能存在
                    ans[k]|=judge[q[k]-tmp[j]]; //如果能用两条路径拼出来，那就存在
        for(int j=0;j<cnt;j++)      //把存在的单条路径长度标上true，供下个子树用
        {
            que.push(tmp[j]);
            judge[tmp[j]]=true;
        }
    }
    while(!que.empty())     //清空judge数组，不要用memset
    {
        judge[que.front()]=false;
        que.pop();
    }
}
//TODO 分治
void divide(int u)
{
    vis[u]=judge[0]=true;   //删除根结点
    solve(u);               //计算经过根结点的路径
    for(int i=Head[u];~i;i=Edge[i].next)    //分治剩余部分
    {
        int v = Edge[i].to;
        if(vis[v]) continue;
        maxp[rt=0]=sum=siz[v];  //把重心置为0，并把maxp[0]置为最大值
        getrt(v,0);
        getrt(rt,0);        //与主函数相同，第二次更新siz大小
        divide(rt);
    }
}
#include <cstring>
int main(int argc, char const *argv[])
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    memset(Head,-1,sizeof(Head));
    read(n,m);
    for(int i=1;i<n;i++)
    {
        int u,v,w;
        read(u,v,w);
        AddEdge(u,v,w);
        AddEdge(v,u,w);
    }
    for(int i=0;i<m;i++) read(q[i]);
    maxp[0]=sum=n;  //maxp[0]置为最大值（一开始rt=0）
    getrt(1,0);     //找重心
    //! 此时siz数组存放的是以1为根时的各树大小，需要以找出的重心为根重算
    getrt(rt,0);
    divide(rt);     //找好重心就可以开始分治了
    for(int i=0;i<m;i++)
    {
        if(ans[i]) puts("AYE");
        else puts("NAY");
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