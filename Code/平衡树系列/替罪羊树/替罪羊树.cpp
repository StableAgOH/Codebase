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
const double alpha = 0.75;
struct Node
{
    int l,r,val;
    int size,fact;
    bool exist;
}tzy[maxn];
int cnt,root;
void newnode(int &now,int val)
{
    now=++cnt;
    tzy[now].val=val;
    tzy[now].size=tzy[now].fact=1;
    tzy[now].exist=true;
}
bool imbalence(int now)
{
    if(std::max(tzy[tzy[now].l].size,tzy[tzy[now].r].size)>tzy[now].size*alpha
       ||tzy[now].size-tzy[now].fact>tzy[now].size*0.3)
        return true;
    return false;
}
#include <vector>
std::vector<int> v;
void ldr(int now)
{
    if(!now) return;
    ldr(tzy[now].l);
    if(tzy[now].exist)
        v.push_back(now);
    ldr(tzy[now].r);
}
void lift(int l,int r,int &now)
{
    if(l==r)
    {
        now=v[l];
        tzy[now].l=tzy[now].r=0;
        tzy[now].size=tzy[now].fact=1;
        return;
    }
    int m = (l+r)>>1;
    while(l<m&&tzy[v[m]].val==tzy[v[m-1]].val)
        m--;
    now = v[m];
    if(l<m) lift(l,m-1,tzy[now].l);
    else tzy[now].l=0;
    lift(m+1,r,tzy[now].r);
    tzy[now].size=tzy[tzy[now].l].size+tzy[tzy[now].r].size+1;
    tzy[now].fact=tzy[tzy[now].l].fact+tzy[tzy[now].r].fact+1;
}
void rebuild(int &now)
{
    v.clear();
    ldr(now);
    if(v.empty())
    {
        now=0;
        return;
    }
    lift(0,v.size()-1,now);
}
void update(int now,int end)
{
    if(!now) return;
    if(tzy[end].val<tzy[now].val)
        update(tzy[now].l,end);
    else update(tzy[now].r,end);
    tzy[now].size=tzy[tzy[now].l].size+tzy[tzy[now].r].size+1;
}
void check(int &now,int end)
{
    if(now==end) return;
    if(imbalence(now))
    {
        rebuild(now);
        update(root,now);
        return;
    }
    if(tzy[end].val<tzy[now].val)
        check(tzy[now].l,end);
    else check(tzy[now].r,end);
}
void ins(int &now,int val)
{
    if(!now)
    {
        newnode(now,val);
        check(root,now);
        return;
    }
    tzy[now].size++;
    tzy[now].fact++;
    if(val<tzy[now].val)
        ins(tzy[now].l,val);
    else ins(tzy[now].r,val);
}
void del(int now,int val)
{
    if(tzy[now].exist&&tzy[now].val==val)
    {
        tzy[now].exist=false;
        tzy[now].fact--;
        check(root,now);
        return;
    }
    tzy[now].fact--;
    if(val<tzy[now].val)
        del(tzy[now].l,val);
    else del(tzy[now].r,val);
}
int getrank(int val)
{
    int now=root,rank=1;
    while(now)
    {
        if(val<=tzy[now].val)
            now=tzy[now].l;
        else 
        {
            rank+=tzy[now].exist+tzy[tzy[now].l].fact;
            now=tzy[now].r;
        }
    }
    return rank;
}
int getnum(int rank)
{
    int now=root;
    while(now)
    {
        if(tzy[now].exist&&tzy[tzy[now].l].fact+tzy[now].exist==rank)
            break;
        else if(tzy[tzy[now].l].fact>=rank)
            now=tzy[now].l;
        else 
        {
            rank-=tzy[tzy[now].l].fact+tzy[now].exist;
            now=tzy[now].r;
        }
    }
    return tzy[now].val;
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
 * ©°©¤©¤©¤©´   ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´
 * ©¦Esc©¦   ©¦ F1©¦ F2©¦ F3©¦ F4©¦ ©¦ F5©¦ F6©¦ F7©¦ F8©¦ ©¦ F9©¦F10©¦F11©¦F12©¦ ©¦P/S©¦S L©¦P/B©¦  ©°©´    ©°©´    ©°©´
 * ©¸©¤©¤©¤©¼   ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼  ©¸©¼    ©¸©¼    ©¸©¼
 * ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´
 * ©¦~ `©¦! 1©¦@ 2©¦# 3©¦$ 4©¦% 5©¦^ 6©¦& 7©¦* 8©¦( 9©¦) 0©¦_ -©¦+ =©¦ BacSp ©¦ ©¦Ins©¦Hom©¦PUp©¦ ©¦Num©¦ / ©¦ * ©¦ - ©¦
 * ©À©¤©¤©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©¤©¤©¤©¤©È ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È
 * ©¦ Tab ©¦ Q ©¦ W ©¦ E ©¦ R ©¦ T ©¦ Y ©¦ U ©¦ I ©¦ O ©¦ P ©¦{ [©¦} ]©¦ | \ ©¦ ©¦Del©¦End©¦PDn©¦ ©¦ 7 ©¦ 8 ©¦ 9 ©¦   ©¦
 * ©À©¤©¤©¤©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©¤©¤©¤©¤©¤©È ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È + ©¦
 * ©¦ Caps ©¦ A ©¦ S ©¦ D ©¦ F ©¦ G ©¦ H ©¦ J ©¦ K ©¦ L ©¦: ;©¦" '©¦ Enter  ©¦               ©¦ 4 ©¦ 5 ©¦ 6 ©¦   ©¦
 * ©À©¤©¤©¤©¤©¤©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¤©È     ©°©¤©¤©¤©´     ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È
 * ©¦ Shift  ©¦ Z ©¦ X ©¦ C ©¦ V ©¦ B ©¦ N ©¦ M ©¦< ,©¦> .©¦? /©¦  Shift   ©¦     ©¦ ¡ü ©¦     ©¦ 1 ©¦ 2 ©¦ 3 ©¦   ©¦
 * ©À©¤©¤©¤©¤©¤©Ð©¤©¤©Ø©¤©Ð©¤©Ø©¤©¤©Ð©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©Ð©Ø©¤©¤©¤©à©¤©¤©¤©Ø©Ð©¤©¤©¤©¤©Ð©¤©¤©¤©¤©È ©°©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©´ ©À©¤©¤©¤©Ø©¤©¤©¤©à©¤©¤©¤©È E©¦©¦
 * ©¦ Ctrl©¦ Win©¦ Alt©¦         Space         ©¦ Alt©¦ Win©¦Menu©¦Ctrl©¦ ©¦ ¡û ©¦ ¡ý ©¦ ¡ú ©¦ ©¦   0   ©¦ . ©¦¡û©¤©¼©¦
 * ©¸©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼
 */
