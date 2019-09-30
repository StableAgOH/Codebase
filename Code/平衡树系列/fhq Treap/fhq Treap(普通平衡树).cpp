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
const int maxn = 1e5+5;
struct Node
{
    int l,r;
    int val,key;
    int size;
}fhq[maxn];
int cnt,root;
#include <random>
std::mt19937 rnd(233);
inline int newnode(int val)
{
    fhq[++cnt].val=val;
    fhq[cnt].key=rnd();
    fhq[cnt].size=1;
    return cnt;
}
inline void update(int now)
{
    fhq[now].size=fhq[fhq[now].l].size+fhq[fhq[now].r].size+1;
}
void split(int now,int val,int &x,int &y)
{
    if(!now) x=y=0;
    else 
    {
        if(fhq[now].val<=val)
        {
            x=now;
            split(fhq[now].r,val,fhq[now].r,y);
        }
        else 
        {
            y=now;
            split(fhq[now].l,val,x,fhq[now].l);
        }
        update(now);
    }
}
int merge(int x,int y)
{
    if(!x||!y) return x+y;
    if(fhq[x].key>fhq[y].key)           // > >= < <=
    {
        fhq[x].r=merge(fhq[x].r,y);
        update(x);
        return x;
    }
    else 
    {
        fhq[y].l=merge(x,fhq[y].l);
        update(y);
        return y;
    }
}
int x,y,z;
inline void ins(int val)
{
    split(root,val,x,y);
    root=merge(merge(x,newnode(val)),y);
}
inline void del(int val)
{
    split(root,val,x,z);
    split(x,val-1,x,y);
    y=merge(fhq[y].l,fhq[y].r);
    root=merge(merge(x,y),z);
}
inline void getrank(int val)
{
    split(root,val-1,x,y);
    print(fhq[x].size+1);
    root=merge(x,y);
}
inline void getnum(int rank)
{
    int now=root;
    while(now)
    {
        if(fhq[fhq[now].l].size+1==rank)
            break;
        else if(fhq[fhq[now].l].size>=rank)
            now=fhq[now].l;
        else 
        {
            rank-=fhq[fhq[now].l].size+1;
            now=fhq[now].r;
        }
    }
    print(fhq[now].val);
}
inline void pre(int val)
{
    split(root,val-1,x,y);
    int now = x;
    while(fhq[now].r)
        now = fhq[now].r;
    print(fhq[now].val);
    root=merge(x,y);
}
inline void nxt(int val)
{
    split(root,val,x,y);
    int now = y;
    while(fhq[now].l)
        now = fhq[now].l;
    print(fhq[now].val);
    root=merge(x,y);
}
int main(int argc, char const *argv[])
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int t;
    read(t);
    while(t--)
    {
        int opt,x;
        read(opt,x);
        switch(opt)
        {
            case 1:
                ins(x);
                break;
            case 2:
                del(x);
                break;
            case 3:
                getrank(x);
                break;
            case 4:
                getnum(x);
                break;
            case 5:
                pre(x);
                break;
            case 6:
                nxt(x);
                break;
        }
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
