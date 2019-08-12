#include <iostream>
#include <string>
using namespace std;
const int maxn = 1e6 + 5;
int nxt[maxn];
int main()
{
    string s1, s2;  //s1是主串s2是子串
    cin >> s1 >> s2;
    //TODO: 构造NEXT数组
    /* 在for里初始化两个指针i和j
       因为i永远是比较“靠右”的
       所以我们要让i不要小于s2的大小
    */
    for (int i = 1, j = 0; i < s2.size(); i++)
    {   //以下纯模拟……
        while (j > 0 && s2[j] != s2[i]) //只要两指针内容不相同就不停地往前跳j
            j = nxt[j - 1];
        if (s2[j] == s2[i])     //内容相同了
            nxt[i] = ++j;
    //这里隐藏了一个j==0但是两指针内容不相同的情况，这种情况nxt[i]=0，所以不用写了
    }
    //TODO: 利用NEXT数组进行KMP匹配
    for (int i = 0, j = 0; i < s1.size();)  //同上个for
    {
        if (s1[i] == s2[j])     //如果当前字符匹配成功
        {
            i++, j++;           //右移两个指针
            if (j != s2.size()) continue;   //如果没匹配到头，那就继续匹配
            cout << i - s2.size() + 1 << endl;  //完全匹配成功，输出位置
        }
        /* 到这里有两种情况：
           一是完全匹配成功了
           二是匹配失败了
           这两种情况是一样的
        */
        j = nxt[j - 1];
        if (s1[i] != s2[j])
            i++;
    }
    for (int i = 0; i < s2.size(); i++)
        cout << nxt[i] << ' ';
    return 0;
}