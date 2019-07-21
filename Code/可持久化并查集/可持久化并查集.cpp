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
const int maxn = 2e5+5;
int n;
struct Node
{
    int l,r,val;
}hjt[maxn*40*2];
int cnt,rootfa[maxn],rootdep[maxn],tot;
void build(int l,int r,int &now)
{
    now = ++cnt;
    if(l==r)
    {
        hjt[now].val=++tot;
        return;
    }
    int m = (l+r)>>1;
    build(l,m,hjt[now].l);
    build(m+1,r,hjt[now].r);
}
void modify(int l,int r,int ver,int &now,int pos,int val)
{
    hjt[now=++cnt]=hjt[ver];
    if(l==r)
    {
        hjt[now].val=val;
        return;
    }
    int m = (l+r)>>1;
    if(pos<=m) modify(l,m,hjt[ver].l,hjt[now].l,pos,val);
    else modify(m+1,r,hjt[ver].r,hjt[now].r,pos,val);
}
int query(int l,int r,int now,int pos)
{
    if(l==r) return hjt[now].val;
    int m = (l+r)>>1;
    if(pos<=m) return query(l,m,hjt[now].l,pos);
    else return query(m+1,r,hjt[now].r,pos);
}
int find(int ver,int x)
{
    int fx = query(1,n,rootfa[ver],x);
    return fx==x?x:find(ver,fx);
}
void merge(int ver,int x,int y)
{
    x = find(ver-1,x);          //ver-1
    y = find(ver-1,y);
    if(x==y)
    {
        rootfa[ver]=rootfa[ver-1];
        rootdep[ver]=rootdep[ver-1];
    }
    else
    {
        int depx = query(1,n,rootdep[ver-1],x);
        int depy = query(1,n,rootdep[ver-1],y);
        if(depx<depy)
        {
            modify(1,n,rootfa[ver-1],rootfa[ver],x,y);
            rootdep[ver]=rootdep[ver-1];
        }
        else if(depx>depy)
        {
            modify(1,n,rootfa[ver-1],rootfa[ver],y,x);
            rootdep[ver]=rootdep[ver-1];
        }
        else
        {
            modify(1,n,rootfa[ver-1],rootfa[ver],x,y);
            modify(1,n,rootdep[ver-1],rootdep[ver],y,depy+1);
        }
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
    int m;
    read(n,m);
    build(1,n,rootfa[0]);
    for(int ver=1;ver<=m;ver++)
    {
        int opt,x,y;
        read(opt);
        switch(opt)
        {
        case 1:
            read(x,y);
            merge(ver,x,y);
            break;
        case 2:
            read(x);
            rootfa[ver]=rootfa[x];
            rootdep[ver]=rootdep[x];
            break;
        case 3:
            read(x,y);
            rootfa[ver]=rootfa[ver-1];
            rootdep[ver]=rootdep[ver-1];
            int fx = find(ver,x);
            int fy = find(ver,y);
            print(fx==fy?1:0);
            break;
        }
    }
    //======================================
    FastIO::flush();
    std::cerr << "Time:" << clock() - c1 << "ms" << std::endl;
    return 0;
}

