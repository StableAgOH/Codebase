#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
//==========================================
#include <set>
#include <tuple>
#include <vector>
#include <algorithm>
const int maxn = 1e5 + 5;
struct Node
{
    int ch[2],val;      // 儿子用数组来存放与Splay有异曲同工之用途
}sgt[70*maxn];
int cnt;
inline int& ls(int k) { return sgt[k].ch[0]; }
inline int& rs(int k) { return sgt[k].ch[1]; }
int modify(int l,int r,int p)       // 单点修改，固定修改p位置为1
{
    int n = ++cnt;
    sgt[n].val=1;
    if(l!=r)
    {
        int m = (l+r)>>1;
        if(p<=m) ls(n) = modify(l, m, p);
        else rs(n) = modify(m+1, r, p);
    }
    return n;
}
void merge(int &x, int y)           // 经典merge，略
{
    if(!x||!y) x|=y;
    else
    {
        sgt[x].val += sgt[y].val;
        merge(ls(x), ls(y));
        merge(rs(x), rs(y));
    }
}
int split(int k, int kth, bool rev)     // 把前kth个数字之外的部分分裂出去并返回结点编号
{
    if(sgt[k].val == kth) return 0;     // 如果不够kth个无法分裂（不会出现<的情况所以==相当于<=）
    int n = ++cnt;
    sgt[n].val = sgt[k].val-kth;        // 分裂出去的部分的数量为总数减kth个
    sgt[k].val = kth;                   // 剩下kth个
    int val = sgt[sgt[k].ch[rev]].val;  // 靠前子树值，如果正序那么就是判断左子树数字数量，如果倒序那么就是判断右子树数字数量
    if(val>=kth)                        // 如果在靠前子树
    {
        sgt[n].ch[rev] = split(sgt[k].ch[rev], kth, rev);   // 分裂靠前子树
        sgt[n].ch[!rev] = sgt[k].ch[!rev];      // 靠前子树只剩kth个，靠后子树自然是要归给新结点（新结点是剩余部分嘛）
        sgt[k].ch[!rev] = 0;                    // 与分出去的结点断开联系
    }
    else sgt[n].ch[!rev] = split(sgt[k].ch[!rev], kth - val, rev);  // 如果在靠后子树，直接分裂即可，因为靠前子树还是应该属于老树
    return n;
}
typedef tuple<int, bool, int> tp3;  // <维护区间的左端点，是否倒序排序，根结点编号>
struct Cmp      // 只以存放的数字个数来去重，不写这个仿函数就会因为继续比较tuple的另两个元素而导致未去重
{
    bool operator() (const tp3 t1, const tp3 t2) const
    {
        return get<0>(t1) < get<0>(t2);
    }
};
set<tp3, Cmp> rt;
// 以下为STL花活取序列
void data(int l, int r, int k, vector<int>& v)
{
    if(!k) return;
    if(l==r) v.push_back(l);
    int m = (l+r)>>1;
    data(l, m, ls(k), v);
    data(m+1, r, rs(k), v);
}
vector<int> print(int l, int r)
{
    vector<int> ret;
    for(auto t : rt)
    {
        vector<int> v;
        data(l, r, get<2>(t), v);
        if(get<1>(t)) ret.insert(ret.end(), v.rbegin(), v.rend());
        else ret.insert(ret.end(), v.begin(), v.end());
    }
    return ret;
}
auto split(int p)   // 使得rt中存在以p为区间左端点的元素
{
    auto it = rt.lower_bound(tp3(p, false, 0));
    if(get<0>(*it)==p) return it;   // 满足条件（当it==rt.end()时也不会满足这个）
    it--;   // 否则p一定在前一个元素中
    int l,n; bool rev;
    tie(l, rev, n) = *it;
    return rt.insert(tp3(p, rev, split(n, p-l, rev))).first;    // 给剩下p-l个元素，其余的分裂成新树，这样p就会是其所在线段树维护区间的左端点了
}
void solve(int l, int r, bool rev)
{
    if(l>r) return;
    auto itl = split(l), itr = split(r+1);  // 良好的习惯：左开右闭区间，便于下面的for操作
    int n = 0;
    for(auto it = itl; it!=itr; ++it)
        merge(n, get<2>(*it));              // 为使他们具有相同的排序顺序，合并起来再塞回去
    rt.erase(itl, itr);                     // 从rt中删去老信息
    rt.insert(tp3(l, rev, n));              // 塞回去
}
signed main(signed argc, char const *argv[])
{
    clock_t c1 = clock();
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    //======================================
    int n,m;
    cin>>n>>m;
    // AcWing3419/蓝桥杯2021省1CB_I 双向排序
    // for(int i=1;i<=n;i++)
    //     rt.insert(tp3(i, false, modify(1, n, i)));
    // while(m--)
    // {
    //     int opt,x;
    //     cin>>opt>>x;
    //     if(opt) solve(x, n, false);
    //     else solve(1, x, true);
    // }
    // for(auto i : print(1, n)) cout<<i<<' ';

    // P2824 [HEOI2016/TJOI2016]排序
    for(int i=1,p; i<=n; i++)
    {
        cin>>p;
        rt.insert(tp3(i, 0, modify(0,n,p)));
    }
    while(m--)
    {
        int opt,l,r;
        cin>>opt>>l>>r;
        solve(l,r,opt);
    }
    int q;
    cin>>q;
    cout<<print(0, n).at(q-1)<<endl;    // 注意vector下标从0开始
    //======================================
end:
    cerr << "Time Used:" << clock() - c1 << "ms" << endl;
    return 0;
}