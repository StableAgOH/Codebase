#include <vector>
#include <functional>
//! 此代码不吸氧必然会跑得很慢,建议搭配氧气使用
class DLX
{
private:
    struct Node{ int l,r,u,d,row,col; };
    std::vector<Node> dcl;                              //十字链表结点
    std::vector<int> s,ans;                             //某列结点个数,答案栈
    bool ms;                                            //是否多解
    std::function<void(std::vector<int>)> process;      //在得到一个解后对答案进行处理的函数

    void remove(int col)                                //删除col列及col列中结点所在行中结点所在列
    {
        dcl[dcl[col].r].l = dcl[col].l;                 //断掉列首结点与其左右结点的关系
        dcl[dcl[col].l].r = dcl[col].r;
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)         //遍历col列所有结点
        {
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)         //遍历col列所有结点所在行的所有结点
            {
                dcl[dcl[j].d].u = dcl[j].u;             //断掉该结点与其上下结点的关系
                dcl[dcl[j].u].d = dcl[j].d;             //注意并没有断掉左右关系,这使得恢复操作变得简单
                s[dcl[j].col]--;                        //该结点所在列的结点个数减1
            }
        }
    }
    void restore(int col)                               //与remove相反
    {
        dcl[dcl[col].r].l = col;
        dcl[dcl[col].l].r = col;
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)
        {
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)
            {
                dcl[dcl[j].d].u = j;
                dcl[dcl[j].u].d = j;
                s[dcl[j].col]++;
            }
        }
    }
    bool dance()
    {
        int col = dcl[0].r;                             //Head右边的结点
        if(!col) return process(ans),true;              //得到了一个解
        bool ret = false;                               //返回值
        for(int i=col;i;i=dcl[i].r)                     //寻找结点数量最小的列,加快求解速度
            if(s[i]<s[col]) col = i;
        remove(col);                                    //第一次删除
        for(int i=dcl[col].d;i!=col;i=dcl[i].d)
        {
            ans.push_back(dcl[i].row);                  //将i结点所在行压入答案栈
            for(int j=dcl[i].r;j!=i;j=dcl[j].r)         //第二次删除
                remove(dcl[j].col);
            bool flag = dance();
            ret |= flag;
            if(flag&&!ms) return true;                  //若只需要一个解则return
            for(int j=dcl[i].l;j!=i;j=dcl[j].l)         //回溯,对应第二次删除
                restore(dcl[j].col);
            ans.pop_back();                             //回溯,弹出已使用的行编号
        }
        restore(col);                                   //回溯,对应第一次删除
        return ret;
    }

public:
    DLX(int col,bool hasMultipleSolutions=false)        //传入列数及是否多解
    {
        ms = hasMultipleSolutions;
        s.resize(col+5);                                //开出空间
        for(int i=0;i<=col;i++)                         //初始化Head结点与列首结点
            dcl.push_back( {i-1,i+1,i,i,0,0} );         //此时有左右而没有上下,注意循环链表,3、4项应为i而不是0
        dcl[0].l = col;                                 //循环链表
        dcl[col].r = 0;
    }
    void addRow(int row,std::vector<int> col)           //为十字链表增加一行,row为行编号,col中存放值为1的列的编号
    {
        int first = dcl.size();                         //暂存行首结点
        for(int c : col)
        {
            int tmp = dcl.size();                       //被push_back的结点的下标
            dcl.push_back( {tmp-1,tmp+1,dcl[c].u,c,row,c} );
            dcl[c].u = dcl[dcl[c].u].d = tmp;           //建立上下关系
            s[c]++;
        }
        dcl[dcl.size()-1].r = first;                    //循环链表
        dcl[first].l = dcl.size()-1;
    }
    bool solve(std::function<void(std::vector<int>)> fun)//传入一个用于处理每个解的答案的函数
    {
        process = fun;
        return dance();
    }
};
