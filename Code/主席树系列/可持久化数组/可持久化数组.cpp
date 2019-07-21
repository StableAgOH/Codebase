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
const int maxn = 1e6+5;
int a[maxn];
struct Node
{
    int l,r,val;
}hjt[maxn*40];
int cnt,root[maxn];
void build(int l,int r,int &now)
{
    now=++cnt;
    if(l==r)
    {
        hjt[now].val=a[l];
        return;
    }
    int m = (l+r)>>1;
    build(l,m,hjt[now].l);
    build(m+1,r,hjt[now].r);
}
void modify(int l,int r,int ver,int &now,int &pos,int &num)
{
    hjt[now=++cnt]=hjt[ver];
    if(l==r)
    {
        hjt[now].val=num;
        return;
    }
    int m = (l+r)>>1;
    if(pos<=m) modify(l,m,hjt[ver].l,hjt[now].l,pos,num);
    else modify(m+1,r,hjt[ver].r,hjt[now].r,pos,num);
}
int query(int l,int r,int now,int &pos)
{
    if(l==r) return hjt[now].val;
    int m = (l+r)>>1;
    if(pos<=m) return query(l,m,hjt[now].l,pos);
    else return query(m+1,r,hjt[now].r,pos);
}
int main(int argc, char const *argv[])
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int n,m,ver,opt,x,y;
    read(n,m);
    for(int i=1;i<=n;i++) read(a[i]);
    build(1,n,root[0]);
    for(int i=1;i<=m;i++)
    {
        read(ver,opt);
        switch(opt)
        {
        case 1:
            read(x,y);
            modify(1,n,root[ver],root[i],x,y);
            break;
        case 2:
            read(x);
            print(query(1,n,root[ver],x));
            root[i]=root[ver];
            break;
        }
    }
    //======================================
    FastIO::flush();
    std::cerr << "Time:" << clock() - c1 << "ms" << std::endl;
    return 0;
}

