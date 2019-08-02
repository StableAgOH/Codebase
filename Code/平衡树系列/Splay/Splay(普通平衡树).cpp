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
    int val,size;
    int cnt;        //当前结点重复次数，默认为1
}spl[maxn];         //内存池
int cnt,root;       //内存池计数器与根节点编号
inline void newnode(int &now,int &val)
{
    spl[now=++cnt].val=val;
    spl[cnt].size++;
    spl[cnt].cnt++;
}
inline void update(int now) //更新size
{
    spl[now].size=spl[spl[now].l].size+spl[spl[now].r].size+spl[now].cnt;
}
inline void zig(int &now)
{
    int l = spl[now].l;
    spl[now].l = spl[l].r;
    spl[l].r = now;
    now = l;
    update(spl[now].r),update(now);
}
inline void zag(int &now)
{
    int r = spl[now].r;
    spl[now].r = spl[r].l;
    spl[r].l = now;
    now = r;
    update(spl[now].l),update(now);
}
void splaying(int x,int &y) //我要把x伸展到y那个位置！
{
    if(x==y) return;        //如果到了终点，return
    int &l = spl[y].l, &r = spl[y].r;   //temp
    if(x==l) zig(y);        //如果左儿子是终点，那就单旋
    else if(x==r) zag(y);   //右儿子是终点也是单旋
    else        //否则就一定是双旋
    {
        if(spl[x].val<spl[y].val)
        {
            if(spl[x].val<spl[l].val)
                splaying(x,spl[l].l),zig(y),zig(y);     //zigzig情况
            else splaying(x,spl[l].r),zag(l),zig(y);    //zagzig情况
        }
        else 
        {
            if(spl[x].val>spl[r].val)
                splaying(x,spl[r].r),zag(y),zag(y);     //zagzag情况
            else splaying(x,spl[r].l),zig(r),zag(y);    //zigzag情况
        }
    }
}
inline void delnode(int now)
{
    splaying(now,root);     //将要删除的结点伸展至根结点
    if(spl[now].cnt>1) spl[now].size--,spl[now].cnt--;  //如果有重复，令重复次数--
    else if(spl[root].r)    //否则如果当前结点有后继
    {
        int p = spl[root].r;
        while(spl[p].l) p=spl[p].l;     //找到后继
        splaying(p,spl[root].r);        //将其伸展至根结点右儿子
        spl[spl[root].r].l=spl[root].l; //右儿子左儿子变为根结点
        root=spl[root].r;               //根结点变为根结点右儿子
        update(root);                   //更新一下size信息
    }
    else root = spl[root].l;    //伸展之后没有后继，说明它是最大的了，那就直接删除
}
void ins(int &now,int &val)
{
    if(!now) newnode(now,val),splaying(now,root);
    else if(val<spl[now].val) ins(spl[now].l,val);
    else if(val>spl[now].val) ins(spl[now].r,val);
    else spl[now].size++,spl[now].cnt++,splaying(now,root); //特判相同的情况
}
void del(int now,int &val)
{
    if(spl[now].val==val) delnode(now);
    else if(val<spl[now].val) del(spl[now].l,val);
    else del(spl[now].r,val);
}
//以下大致与以前的代码相同，有大变动的地方给出了注释
int getrank(int val)
{
    int now = root, rank = 1;
    while(now)
    {
        if(spl[now].val==val)   //找到了要的结点，这个之前的没有
        {
            rank+=spl[spl[now].l].size;
            splaying(now,root);
            break;
        }
        if(val<=spl[now].val)
            now=spl[now].l;
        else 
        {
            rank+=spl[spl[now].l].size+spl[now].cnt;
            now=spl[now].r;
        }
    }
    return rank;
}
int getnum(int rank)
{
    int now = root;
    while(now)
    {
        int lsize = spl[spl[now].l].size;
        if(lsize+1<=rank&&rank<=lsize+spl[now].cnt) //如果在这个范围内，那就是当前结点
        {
            splaying(now,root);
            break;
        }
        else if(lsize>=rank)
            now=spl[now].l;
        else 
        {
            rank-=lsize+spl[now].cnt;
            now=spl[now].r;
        }
    }
    return spl[now].val;
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
            ins(root,x);
            break;
        case 2:
            del(root,x);
            break;
        case 3: 
            print(getrank(x));
            break;
        case 4:
            print(getnum(x));
            break;
        case 5:
            print(getnum(getrank(x)-1));
            break;
        case 6:
            print(getnum(getrank(x+1)));
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