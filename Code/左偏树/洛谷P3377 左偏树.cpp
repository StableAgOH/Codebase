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
const int maxn = 1e5+5;
struct Node
{
    int l,r,fa;     //l,r分别为左右儿子编号;fa为并查集,不属于左偏树，不属于左偏树，不属于左偏树
    int val,dis;    //val为结点键值,val==-1代表结点被删除;dis为结点的距离
}ltt[maxn];         //内存池写法，与平衡树主席树相同
#define ls(x) ltt[x].l  //懒人专用define
#define rs(x) ltt[x].r
inline int merge(int x,int y)   //合并两堆，x,y都是堆顶元素的编号
{
    if(!x||!y) return x+y;  //如果有空的返回另一个，与fhq-Treap相同
    //? 或前语句是为了维护小根堆性质，或后语句是题目要求的（值相同则下标小的优先级高）
    if(ltt[x].val>ltt[y].val||(ltt[x].val==ltt[y].val&&x>y))
        std::swap(x,y);
    rs(x)=merge(rs(x),y);   //合并右子树和y
    ltt[rs(x)].fa=x;        //维护并查集，x的右儿子的父亲肯定是x
    if(ltt[ls(x)].dis<ltt[rs(x)].dis)   //如果不满足左偏树的性质了那就交换左右儿子
        std::swap(ls(x),rs(x));
    ltt[x].dis=ltt[rs(x)].dis+1;        //利用结点距离等于右儿子距离+1来更新dis
    return x;                           //return合并好后的堆顶结点编号
}
int find(int x) { return ltt[x].fa==x?x:ltt[x].fa=find(ltt[x].fa); }    //并查集&&路径压缩
inline void pop(int x)  //删除堆顶元素
{
    ltt[x].val=-1;          //值置为-1代表被删除
    ltt[ls(x)].fa=ls(x);    //维护并查集（一个结点的父亲是结点本身，代表结点没有父亲了）
    ltt[rs(x)].fa=rs(x);    //维护并查集
    //? 因为路径压缩，所以可能会有除了ls(x)和rs(x)以外的结点的fa指针指向x
    //? 所以要这样子写，不能让并查集断掉：
    ltt[x].fa=merge(ls(x),rs(x));
}
int main(int argc, char const *argv[])
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int n,m;
    read(n,m);
    ltt[0].dis=-1;
    for(int i=1;i<=n;i++)
    {
        read(ltt[i].val);   //读入值
        ltt[i].fa=i;        //初始化并查集
    }
    while(m--)
    {
        int opt,x,y;
        read(opt,x);
        if(opt==1)
        {
            read(y);
            if(ltt[x].val==-1||ltt[y].val==-1)  //如果有一个被删除了
                continue;
            int fx=find(x),fy=find(y);          //利用并查集查找堆顶结点编号
            if(fx==fy) continue;                //如果已经在同一堆中
            ltt[fx].fa=ltt[fy].fa=merge(fx,fy); //合并
        }
        else 
        {
            if(ltt[x].val==-1) print(-1);       //如果被删除了
            else 
            {
                int fx = find(x);               //利用并查集查找堆顶结点编号
                print(ltt[fx].val);             //输出堆顶元素的值
                pop(fx);                        //并且弹出
            }
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