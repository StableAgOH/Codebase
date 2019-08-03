#include <iostream>
#include <ctime>
#include <cstdio>
#include <cctype>
namespace FastIO
{
char buf[1 << 21], buf2[1 << 21], a[20], *p1 = buf, *p2 = buf, hh = '\n';
int p, p3 = -1;
void read() {}
void print() {}
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
    buf2[++p3] = hh;
    print(oth...);
}
} // namespace FastIO
#define read FastIO::read
#define print FastIO::print
//======================================
const int maxn = 5e5+5;
struct E
{
    int to,next;
}Edge[maxn*2];
int tot,Head[maxn];
inline void AddEdge(int u,int v)
{
    Edge[tot].to=v,Edge[tot].next=Head[u],Head[u]=tot++;
    Edge[tot].to=u,Edge[tot].next=Head[v],Head[v]=tot++;
}
int fa[maxn],siz[maxn],son[maxn],dep[maxn];
void dfs1(int u,int f)
{
    fa[u]=f,siz[u]=1,dep[u]=dep[f]+1;
    int maxson = -1;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int &v = Edge[i].to;
        if(v==f) continue;
        dfs1(v,u);
        siz[u]+=siz[v];
        if(siz[v]>maxson)
            maxson=siz[v],son[u]=v;
    }
}
int top[maxn];
void dfs2(int u,int t)
{
    top[u]=t;
    if(!son[u]) return;
    dfs2(son[u],t);
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int &v = Edge[i].to;
        if(v==fa[u]||v==son[u]) continue;
        dfs2(v,v);
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
    int n,m,s;
    read(n,m,s);
    while(--n)
    {
        int u,v;
        read(u,v);
        AddEdge(u,v);
    }
    dfs1(s,s);
    dfs2(s,s);
    while(m--)
    {
        int u,v;
        read(u,v);
        while(top[u]!=top[v])
        {
            if(dep[top[u]]<dep[top[v]]) std::swap(u,v);
            u=fa[top[u]];
        }
        print(dep[u]<dep[v]?u:v);
    }
    //======================================
    FastIO::flush();
    std::cerr << "Time:" << clock() - c1 << "ms" << std::endl;
    return 0;
}