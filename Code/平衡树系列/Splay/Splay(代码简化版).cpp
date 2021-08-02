#include <iostream>
#include <ctime>
using namespace std;
//==========================================
const int maxn = 1e5 + 5;
struct Node
{
    int fa, ch[2], val, size; //ch[0]是左儿子，ch[1]是右儿子
} spl[maxn];
int cnt, root;
inline void update(int x)
{
    spl[x].size = spl[spl[x].ch[0]].size + spl[spl[x].ch[1]].size + 1;
}
inline bool ident(int x, int f) { return spl[f].ch[1] == x; } //查询父子关系
inline void connect(int x, int f, int s)                      //建立父子关系
{
    spl[f].ch[s] = x;
    spl[x].fa = f;
}
void rotate(int x) //合二为一的旋转
{
    int f = spl[x].fa, ff = spl[f].fa, k = ident(x, f);
    connect(spl[x].ch[k ^ 1], f, k); //三次建立父子关系
    connect(x, ff, ident(f, ff));
    connect(f, x, k ^ 1);
    update(f), update(x); //别忘了更新大小信息
}
void splaying(int x, int top) //代表把x转到top的儿子，top为0则转到根结点
{
    if (!top)
        root = x;
    while (spl[x].fa != top)
    {
        int f = spl[x].fa, ff = spl[f].fa;
        if (ff != top)
            ident(f, ff) ^ ident(x, f) ? rotate(x) : rotate(f);
        rotate(x); //最后一次都是旋转x
    }
}
void newnode(int &now, int fa, int val) //新建节点，要注意fa指针的初始化
{
    spl[now = ++cnt].val = val;
    spl[cnt].fa = fa;
    spl[cnt].size = 1;
}
void delnode(int x) //删除结点，要注意fa指针的维护
{
    splaying(x, 0);
    if (spl[x].ch[1])
    {
        int p = spl[x].ch[1];
        while (spl[p].ch[0])
            p = spl[p].ch[0];
        splaying(p, x);
        connect(spl[x].ch[0], p, 0);
        root = p;
        spl[p].fa = 0;
        update(root);
    }
    else
        root = spl[x].ch[0], spl[root].fa = 0;
}
void ins(int val, int &now = root, int fa = 0) //递归式，要传fa指针
{
    if (!now)
        newnode(now, fa, val), splaying(now, 0);
    else if (val < spl[now].val)
        ins(val, spl[now].ch[0], now);
    else
        ins(val, spl[now].ch[1], now);
}
void del(int val, int now = root) //同上
{
    if (val == spl[now].val)
        delnode(now);
    else if (val < spl[now].val)
        del(val, spl[now].ch[0]);
    else
        del(val, spl[now].ch[1]);
}
int getrank(int val)
{
    int now = root, rank = 1, pre = 0;
    while (now)
    {
        if (val <= spl[now].val)
        {
            pre = now;
            now = spl[now].ch[0];
        }
        else
        {
            rank += spl[spl[now].ch[0]].size + 1;
            now = spl[now].ch[1];
        }
    }
    if (pre)
        splaying(pre, 0);
    return rank;
}
int getnum(int rank)
{
    int now = root;
    while (now)
    {
        if (spl[spl[now].ch[0]].size + 1 == rank)
        {
            splaying(now, 0);
            break;
        }
        else if (spl[spl[now].ch[0]].size >= rank)
            now = spl[now].ch[0];
        else
        {
            rank -= spl[spl[now].ch[0]].size + 1;
            now = spl[now].ch[1];
        }
    }
    return spl[now].val;
}
signed main(signed argc, char const *argv[])
{
    clock_t c1 = clock();
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    //======================================
    int t;
    cin >> t;
    while (t--)
    {
        int opt, x;
        cin >> opt >> x;
        switch (opt)
        {
        case 1:
            ins(x);
            break;
        case 2:
            del(x);
            break;
        case 3:
            cout << getrank(x) << '\n';
            break;
        case 4:
            cout << getnum(x) << '\n';
            break;
        case 5:
            cout << getnum(getrank(x) - 1) << '\n';
            break;
        case 6:
            cout << getnum(getrank(x + 1)) << '\n';
            break;
        }
    }
    //======================================
#ifdef LOCAL
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
#endif
    return 0;
}