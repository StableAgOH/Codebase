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
const int maxn = 1e5 + 5;
const int maxm = maxn * 2;
struct E
{
    int to, next;
} Edge[maxm];
int tot, Head[maxn];
inline void AddEdge(int u, int v)
{
    Edge[tot] = (E){v, Head[u]};
    Head[u] = tot++;
    Edge[tot] = (E){u, Head[v]};
    Head[v] = tot++;
}
int mod;
int v[maxn];
int fa[maxn], dep[maxn], siz[maxn], son[maxn];
void dfs1(int u, int f)
{
    fa[u] = f;
    dep[u] = dep[f] + 1;
    siz[u] = 1;
    int maxsize = -1;
    for (int i = Head[u]; ~i; i = Edge[i].next)
    {
        int v = Edge[i].to;
        if (v == f)
            continue;
        dfs1(v, u);
        siz[u] += siz[v];
        if (siz[v] > maxsize)
        {
            maxsize = siz[v];
            son[u] = v;
        }
    }
}
int tim, dfn[maxn], top[maxn], w[maxn];
void dfs2(int u, int t)
{
    dfn[u] = ++tim;
    top[u] = t;
    w[tim] = v[u];
    if (!son[u])
        return;
    dfs2(son[u], t);
    for (int i = Head[u]; ~i; i = Edge[i].next)
    {
        int v = Edge[i].to;
        if (v == fa[u] || v == son[u])
            continue;
        dfs2(v, v);
    }
}
struct Node
{
    int l, r, f, val;
} sgt[maxn * 4];
inline int ls(int k) { return k << 1; }
inline int rs(int k) { return k << 1 | 1; }
inline void pushup(int k) { sgt[k].val = (sgt[ls(k)].val + sgt[rs(k)].val) % mod; }
inline void pushdown(int k)
{
    sgt[ls(k)].f += sgt[k].f;
    sgt[rs(k)].f += sgt[k].f;
    sgt[ls(k)].val += (sgt[ls(k)].r - sgt[ls(k)].l + 1) * sgt[k].f % mod;
    sgt[rs(k)].val += (sgt[rs(k)].r - sgt[rs(k)].l + 1) * sgt[k].f % mod;
    sgt[k].f = 0;
}
void build(int l, int r, int k = 1)
{
    sgt[k].l = l, sgt[k].r = r;
    if (l == r)
    {
        sgt[k].val = w[l] % mod;
        return;
    }
    int m = (l + r) >> 1;
    build(l, m, ls(k));
    build(m + 1, r, rs(k));
    pushup(k);
}
void modify(int x, int y, int z, int k = 1)
{
    int l = sgt[k].l, r = sgt[k].r;
    if (x <= l && y >= r)
    {
        sgt[k].f += z;
        sgt[k].val += (r - l + 1) * z;
        sgt[k].val %= mod;
        return;
    }
    if (sgt[k].f)
        pushdown(k);
    int m = (l + r) >> 1;
    if (x <= m)
        modify(x, y, z, ls(k));
    if (y > m)
        modify(x, y, z, rs(k));
    pushup(k);
}
int query(int x, int y, int k = 1)
{
    int l = sgt[k].l, r = sgt[k].r;
    if (x <= l && y >= r)
        return sgt[k].val;
    if (sgt[k].f)
        pushdown(k);
    int sum = 0, m = (l + r) >> 1;
    if (x <= m)
        sum += query(x, y, ls(k));
    if (y > m)
        sum += query(x, y, rs(k));
    return sum % mod;
}
void mchain(int x, int y, int z)
{
    z %= mod;
    while (top[x] != top[y])
    {
        if (dep[top[x]] < dep[top[y]])
            std::swap(x, y);
        modify(dfn[top[x]], dfn[x], z);
        x = fa[top[x]];
    }
    if (dep[x] > dep[y])
        std::swap(x, y);
    modify(dfn[x], dfn[y], z);
}
int qchain(int x, int y)
{
    int ret = 0;
    while (top[x] != top[y])
    {
        if (dep[top[x]] < dep[top[y]])
            std::swap(x, y);
        ret += query(dfn[top[x]], dfn[x]);
        x = fa[top[x]];
    }
    if (dep[x] > dep[y])
        std::swap(x, y);
    ret += query(dfn[x], dfn[y]);
    return ret % mod;
}
inline void mson(int x, int z)
{
    modify(dfn[x], dfn[x] + siz[x] - 1, z);
}
inline int qson(int x)
{
    return query(dfn[x], dfn[x] + siz[x] - 1);
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
    memset(Head, -1, sizeof(Head));
    int n, m, r;
    read(n, m, r, mod);
    for (int i = 1; i <= n; i++)
        read(v[i]);
    for (int i = 1; i < n; i++)
    {
        int u, v;
        read(u, v);
        AddEdge(u, v);
    }
    dfs1(r, r);
    dfs2(r, r);
    build(1, n);
    while (m--)
    {
        int opt, x, y, z;
        read(opt);
        switch (opt)
        {
        case 1:
            read(x, y, z);
            mchain(x, y, z);
            break;
        case 2:
            read(x, y);
            print(qchain(x, y));
            break;
        case 3:
            read(x, z);
            mson(x, z);
            break;
        case 4:
            read(x);
            print(qson(x));
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