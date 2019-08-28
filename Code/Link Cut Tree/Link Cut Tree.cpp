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
    int fa,ch[2],val,res;   //res是异或结果
    bool tag;               //翻转懒标记
}spl[maxn];
//因为被毒瘤数据卡得TLE一个点，所以全部换成了#define。都是字面意思
#define ls(x) (spl[x].ch[0])
#define rs(x) (spl[x].ch[1])
#define fa(x) (spl[x].fa)
#define ident(x,f) (rs(f)==x)       //和下面的connect都是Splay的辅助函数
#define connect(x,f,s) spl[fa(x)=f].ch[s]=x
#define update(x) spl[x].res=spl[ls(x)].res^spl[rs(x)].res^spl[x].val
#define ntroot(x) (ls(fa(x))==x||rs(fa(x))==x)  //判断结点是否为Splay的根
#define reverse(x) std::swap(ls(x),rs(x)),spl[x].tag^=1
inline void pushdw(int x)           //懒标记下传
{
    if(spl[x].tag)
    {
        if(ls(x)) reverse(ls(x));
        if(rs(x)) reverse(rs(x));
    }
    spl[x].tag=0;
}
void pushall(int x)                 //头递归，从上到下下传所有懒标记
{
    if(ntroot(x)) pushall(fa(x));
    pushdw(x);
}
inline void rotate(int x)           //Splay基操
{
    int f=fa(x),ff=fa(f),k=ident(x,f);
    connect(spl[x].ch[k^1],f,k);
    fa(x)=ff;
    if(ntroot(f)) spl[ff].ch[ident(f,ff)]=x;//※重要，不能忘记判断，关系到虚实边
    connect(f,x,k^1);
    update(f),update(x);
}
inline void splaying(int x)         //Splay基操，都是伸展到根结点
{
    pushall(x);                     //要先把上面的懒标记全都下传
    while(ntroot(x))
    {
        int f=fa(x),ff=fa(f);
        if(ntroot(f)) ident(f,ff)^ident(x,f)?rotate(x):rotate(f);
        rotate(x);
    }
}
inline void access(int x)           //从x到原树根结点拉一条实链
{
    for(int y=0;x;x=fa(y=x))        //y为上一个Splay的根
    {
        splaying(x);                //伸展到当前Splay的根
        rs(x)=y;                    //右儿子连上上一个Splay的根
        update(x);                  //别忘更新＞﹏＜
    }
}
inline void mkroot(int x)           //给原树换根
{
    access(x);                      //先拉实链，拉好后x一定在Splay的最右（深度最大）
    splaying(x);                    //再伸展，伸展后x必定没有右儿子
    reverse(x);                     //翻转拉出来这条实链，使深度顺序翻转
}
inline int findroot(int x)          //寻找结点在原树的根
{
    access(x);                      //先拉实链
    splaying(x);                    //再伸展
    while(ls(x))                    //因为根结点必定深度最小，所以不停往左找就OK了
    {
        pushdw(x);                  //别忘了下传，第一个儿子是没问题的但是第二个往后……
        x=ls(x);
    }
    splaying(x);                    //用来保证时间复杂度，防止卡链
    return x;
}
inline void link(int x,int y)       //连边，不保证数据合法
{
    mkroot(x);                      //换根
    if(findroot(y)==x) return;      //如果y所在的树的根结点是x，那说明两者在一棵树上
    fa(x)=y;
}
inline void cut(int x,int y)        //断边，不保证数据合法
{
    mkroot(x);                      //换根
    //? 如果y跟x不在一棵树上 or x和y之间不是紧紧挨着的，return
    //! 注意这里findroot后由于保证复杂度的一句伸展，导致刚才被换成根的x成为了Splay的根结点
    //* 又因为x在原树中是根结点，深度最小，所以在Splay中一定是x为根结点y为其右儿子
    if(findroot(y)!=x||fa(y)!=x||ls(y)) return;
    fa(y)=rs(x)=0;                  //双向断边
    update(x);                      //别忘更新＞﹏＜
}
inline void split(int x,int y)      //把x--y的路径拆出来
{
    mkroot(x);                      //换根
    access(y);                      //拉实链
    splaying(y);                    //伸展
    //? 此时y必定没有右儿子且左儿子是一条到x的实链，所以访问y就可以作任何关于这条链的操作了
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
    //? 刚开始的时候所有点之间都没连着
    for(int i=1;i<=n;i++) read(spl[i].val);
    while(m--)
    {
        int opt,x,y;
        read(opt,x,y);
        switch(opt)
        {
        case 0:
            split(x,y);
            print(spl[y].res);      //访问y就相当于访问这条链了
            break;
        case 1:
            link(x,y);
            break;
        case 2:
            cut(x,y);
            break;
        case 3:
            splaying(x);            //注意要先伸展到根，否则会很麻烦
            spl[x].val=y;
            update(x);              //不更新也没啥问题，加不加都行
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