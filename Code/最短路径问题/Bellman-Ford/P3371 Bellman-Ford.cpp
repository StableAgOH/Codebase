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
const int maxm = 5e5+5;
struct E
{
    int u,v,w;
}Edge[maxm];
long long dis[maxn];
#include <algorithm>
int main(int argc, char const *argv[])
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int n,m,s;
    read(n,m,s);
    for(int i=0;i<m;i++)
        read(Edge[i].u,Edge[i].v,Edge[i].w);    //Bellman-Ford算法不需要链式前向星存图
    std::fill(dis+1,dis+1+n,2147483647);//两句初始化，因为这里用的int的最大值，松弛操作有加法，所以dis要开longlong
    dis[s]=0;
    bool flag;
    for(int i=1;i<n;i++)    //Bellman-Ford板子：|V|-1次对所有边进行松弛操作
    {
        flag = true;
        for(int j=0;j<m;j++)    //遍历所有边
        {
            if(dis[Edge[j].u]+Edge[j].w<dis[Edge[j].v])     //松弛操作
            {
                dis[Edge[j].v]=dis[Edge[j].u]+Edge[j].w;
                flag = false;       //如果有更新，置flag为false
            }
        }
        if(flag) break; //如果一次遍历所有边都没有更新dis值，那显然之后遍历所有边也不会更新dis值了，直接跳出即可
    }
    for(int i=1;i<=n;i++)
        printf("%lld ",dis[i]);
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