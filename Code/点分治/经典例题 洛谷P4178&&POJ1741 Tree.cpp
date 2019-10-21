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
const int maxn = 4e4+5;
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
int n,k;
int siz[maxn],maxp[maxn],rt,sum;
bool vis[maxn];
void getrt(int u,int f)
{
    siz[u]=1,maxp[u]=0;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==f||vis[v]) continue;
        getrt(v,u);
        siz[u]+=siz[v];
        if(siz[v]>maxp[u]) maxp[u]=siz[v];
    }
    maxp[u]=std::max(maxp[u],sum-siz[u]);
    if(maxp[u]<maxp[rt]) rt=u;
}
const int maxk = 2e4+5;
struct Node
{
    int l,r,val;
}sgt[maxk<<2];
#define ls(k) (k<<1)
#define rs(k) (k<<1|1)
#define pushup(k) sgt[k].val=sgt[ls(k)].val+sgt[rs(k)].val
void build(int l,int r,int k=1)
{
    sgt[k].l=l,sgt[k].r=r;
    if(l==r) return;
    int m = (l+r)>>1;
    build(l,m,ls(k));
    build(m+1,r,rs(k));
}
void modify(int x,int z,int k=1)
{
    int l=sgt[k].l,r=sgt[k].r;
    if(l==r)
    {
        sgt[k].val+=z;
        return;
    }
    int m = (l+r)>>1;
    if(x<=m) modify(x,z,ls(k));
    else modify(x,z,rs(k));
    pushup(k);
}
int query(int x,int y,int k=1)
{
    if(x>y) return 0; 
    int l=sgt[k].l,r=sgt[k].r;
    if(x<=l&&y>=r) return sgt[k].val;
    int m = (l+r)>>1;
    int sum = 0;
    if(x<=m) sum+=query(x,y,ls(k));
    if(y>m) sum+=query(x,y,rs(k));
    return sum;
}
int cnt,tmp[maxn],dis[maxn];
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
int ans;
void solve(int u)
{
    static std::queue<int> q;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(vis[v]) continue;
        cnt=0;
        dis[v]=Edge[i].w;
        getdis(v,u);
        for(int j=0;j<cnt;j++)
            if(k>=tmp[j])
                ans+=query(1,k-tmp[j])+1;
            // else if(k==tmp[j]) ans++;
        for(int j=0;j<cnt;j++)
        {
            q.push(tmp[j]);
            modify(tmp[j],1);
        }
    }
    while(!q.empty())
    {
        modify(q.front(),-1);
        q.pop();
    }
}
void divide(int u)
{
    vis[u]=true;
    solve(u);
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(vis[v]) continue;
        maxp[rt=0]=sum=siz[v];
        getrt(v,0);
        getrt(rt,0);
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
    read(n);
    for(int i=1;i<n;i++)
    {
        int u,v,w;
        read(u,v,w);
        AddEdge(u,v,w);
        AddEdge(v,u,w);
    }
    read(k);
    build(1,20000);
    maxp[0]=sum=n;
    getrt(1,0);
    getrt(rt,0);
    divide(rt);
    print(ans);
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