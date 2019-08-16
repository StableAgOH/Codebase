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
#include <algorithm>
const int maxn = 2e5 + 5;   //因为一个矩形有两边所以数据范围要乘2
int v[maxn];                //存放所有横边的y坐标
struct L                    //四元组
{
    int x;      //竖边的x坐标
    int y1,y2;  //竖边的y坐标，y1较小
    int state;  //是左边还是右边
    bool operator<(L oth) { return x<oth.x; }   //排序时使用x坐标排序
}line[maxn];
struct Node         //线段树
{
    int l,r;        //结点区间左右
    int cover;      //被覆盖次数
    long long len;  //区间真实长度
}sgt[maxn<<3];      //注意这个大小
inline int ls(int k) { return k<<1; }   //取左儿子
inline int rs(int k) { return k<<1|1; } //取右儿子
inline void pushup(int k)       //与普通线段树的区别
{
    if(sgt[k].cover) sgt[k].len=sgt[k].r-sgt[k].l;
    else sgt[k].len=sgt[ls(k)].len+sgt[rs(k)].len;
}
void build(int l,int r,int k=1)
{
    sgt[k].l=v[l],sgt[k].r=v[r];//与普通线段树的区别
    if(r-l<=1) return;          //与普通线段树的区别
    int m = (l+r)>>1;
    build(l,m,ls(k));
    build(m,r,rs(k));
}
void modify(int x,int y,int z,int k=1)
{
    int l=sgt[k].l,r=sgt[k].r;
    if(x<=l&&y>=r)
    {
        sgt[k].cover+=z;
        pushup(k);              //与普通线段树的区别
        return;
    }
    if(x<sgt[ls(k)].r) modify(x,y,z,ls(k)); //与普通线段树的区别
    if(y>sgt[rs(k)].l) modify(x,y,z,rs(k)); //与普通线段树的区别
    pushup(k);
}
int main(int argc, char const *argv[])
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    clock_t c1 = clock();
    //======================================
    int a,b,c,d,n;
    read(n);
    for(int i=1;i<=n;i++)       //读入数据，位置无所谓，待会要排序
    {
        read(a,b,c,d);
        v[i]=b,v[n+i]=d;
        line[i]=(L){a,b,d,1},line[n+i]=(L){c,b,d,-1};
    }
    std::sort(v+1,v+1+(n<<1));
    std::sort(line+1,line+1+(n<<1));
    build(1,n<<1);
    unsigned long long ans = 0; //爆int的毒瘤数据只好用ull处理
    for(int i=1;i<=n<<1;i++)
    {
        ans += sgt[1].len*(line[i].x-line[i-1].x);  //注意两语句顺序
        modify(line[i].y1,line[i].y2,line[i].state);
    }
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