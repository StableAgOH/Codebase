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
    int val;
    int height,size;
}avl[maxn];         //内存池，老生常谈
int cnt,root;
inline void newnode(int &now,int val)   //新建结点，老生常谈
{
    avl[now=++cnt].val=val;
    avl[cnt].size=1;
}
inline void update(int now)     //更新节点信息
{
    avl[now].size=avl[avl[now].l].size+avl[avl[now].r].size+1;
    avl[now].height=std::max(avl[avl[now].l].height,avl[avl[now].r].height)+1;
}
inline int factor(int now)      //获取BF
{
    return avl[avl[now].l].height-avl[avl[now].r].height;
}
inline void lrotate(int &now)   //左旋
{
    int r = avl[now].r;
    avl[now].r = avl[r].l;
    avl[r].l = now;
    now = r;
    update(avl[now].l),update(now);
}
inline void rrotate(int &now)   //右旋
{
    int l = avl[now].l;
    avl[now].l = avl[l].r;
    avl[l].r = now;
    now = l;
    update(avl[now].r),update(now);
}
inline void check(int &now)//检查是否需要旋转，并且更新节点信息
{
    int nf = factor(now);
    if(nf>1)
    {
        int lf = factor(avl[now].l);
        if(lf>0) rrotate(now);                      //LL
        else lrotate(avl[now].l),rrotate(now);      //LR
    }
    else if(nf<-1)
    {
        int rf = factor(avl[now].r);
        if(rf<0) lrotate(now);                      //RR
        else rrotate(avl[now].r),lrotate(now);      //RL
    }
    else if(now) update(now);//如果平衡并且非空，那就直接更新
}
void ins(int &now,int val)
{
    if(!now) newnode(now,val);      //到了一个不存在的点，新建它
    else if(val<avl[now].val) ins(avl[now].l,val);  //小的往左
    else ins(avl[now].r,val);                       //大的往右
    check(now);                     //递归回溯检查平衡
}
int find(int &now,int fa)
{
    int ret;
    if(!avl[now].l)     //找到了要的后继
    {
        ret = now;      //返回值就是你了
        avl[fa].l=avl[now].r;//父亲的左儿改成该点的右儿
    }
    else                //没找到
    {
        ret = find(avl[now].l,now); //返回值是继续找
        check(now);     //递归回溯过程中检查是否需要调整
    }
    return ret;         //把找到的结点编号返回
}
void del(int &now,int val)
{
    if(val==avl[now].val)   //要删除的结点
    {
        int l = avl[now].l,r = avl[now].r;  //temp
        if(!l||!r) now=l+r;  //如果无儿或单儿，很简单
        else 
        {
            now = find(r,r);    //找后继，替换当前
            if(now!=r)          //如果后继不是原来的右儿
                avl[now].r=r;   //右儿改成原来的右儿
            avl[now].l=l;       //左儿连上
        }
    }
    else if(val<avl[now].val) del(avl[now].l,val);
    else del(avl[now].r,val);
    check(now);                 //递归回溯检查
}
void ldr(int now)               //调试用的中序遍历
{
    if(!now) return;
    ldr(avl[now].l);
    print(avl[now].val);
    ldr(avl[now].r);
}
int getrank(int val)            //以下与替罪羊树同
{
    int now=root,rank=1;
    while(now)
    {
        if(val<=avl[now].val)
            now=avl[now].l;
        else
        {
            rank+=avl[avl[now].l].size+1;
            now=avl[now].r;
        }
    }
    return rank;
}
int getnum(int rank)
{
    int now=root;
    while(now)
    {
        if(avl[avl[now].l].size+1==rank)
            break;
        else if(avl[avl[now].l].size>=rank)
            now=avl[now].l;
        else 
        {
            rank-=avl[avl[now].l].size+1;
            now=avl[now].r;
        }
    }
    return avl[now].val;
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