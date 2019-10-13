#include <cstdio>
#include <cstring>
const int maxn = 1e5+5;
struct E
{
    int to,next;
}Edge[maxn<<1];
int tot,Head[maxn];
inline void AddEdge(int u,int v)
{
    Edge[tot].to=v;
    Edge[tot].next=Head[u];
    Head[u]=tot++;
}
int siz[maxn],son[maxn];
void dfs(int u,int f)           //与重链剖分相同的写法找重儿子
{
    siz[u]=1;
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==f) continue;
        dfs(v,u);
        siz[u]+=siz[v];
        if(siz[v]>siz[son[u]])
            son[u]=v;
    }
}
int col[maxn],cnt[maxn];    //col存放某结点的颜色，cnt存放某颜色在“当前”子树中的数量
long long ans[maxn],sum;    //ans是答案数组，sum用于累加计算出“当前”子树的答案
int flag,maxc;              //flag用于标记重儿子，maxc用于更新最大值
//TODO 统计某结点及其所有轻儿子的贡献
void count(int u,int f,int val)
{
    cnt[col[u]]+=val;//val为正为负可以控制是增加贡献还是删除贡献
    if(cnt[col[u]]>maxc)    //找最大值，基操吧
    {
        maxc=cnt[col[u]];
        sum=col[u];
    }
    else if(cnt[col[u]]==maxc)  //这样做是因为如果两个颜色数量相同那都得算
        sum+=col[u];
    for(int i=Head[u];~i;i=Edge[i].next) //排除被标记的重儿子，统计其它儿子子树信息
    {
        int v = Edge[i].to;
        if(v==f||v==flag) continue; //不能写if(v==f||v==son[u]) continue;
        count(v,u,val);
    }
}
//dsu on tree的板子
void dfs(int u,int f,bool keep)
{
    //* 第一步：搞轻儿子及其子树算其答案删贡献
    for(int i=Head[u];~i;i=Edge[i].next)
    {
        int v = Edge[i].to;
        if(v==f||v==son[u]) continue;
        dfs(v,u,false);
    }
    //* 第二步：搞重儿子及其子树算其答案不删贡献
    if(son[u])
    {
        dfs(son[u],u,true);
        flag = son[u];
    }
    //* 第三步：暴力统计u及其所有轻儿子的贡献合并到刚算出的重儿子信息里
    count(u,f,1);
    flag = 0;   
    ans[u] = sum;
    //* 把需要删除贡献的删一删
    if(!keep)
    {
        count(u,f,-1);
        sum=maxc=0; //这是因为count函数中会改变这两个变量值
    }
}
int main()
{
#ifndef ONLINE_JUDGE
    freopen("in.in","r",stdin);
    freopen("out.out","w",stdout);
#endif
    memset(Head,-1,sizeof(Head));
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        scanf("%d",col+i);
    for(int i=1,u,v;i<n;i++)
    {
        scanf("%d%d",&u,&v);
        AddEdge(u,v);
        AddEdge(v,u);
    }
    dfs(1,0);
    dfs(1,0,0);
    for(int i=1;i<=n;i++)
        printf("%I64d ",ans[i]);
    return 0;
}