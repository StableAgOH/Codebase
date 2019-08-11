//€€₣版本
#include<iostream>
using namespace std;
int x, p, m, i, result;
int main()
{
	cin >> x >> p >> m;
	result = 1;
	while (p)
    {
		if (p % 2 == 1)
			result = result * x % m;
		p /= 2;
		x = x * x % m;
	}
	cout << result << endl;
	return 0;
}
//压行狂魔版本
template <typename T>
T qpow(T a, T k, T m)
{
    auto ans = a;
    for(k--;k>0;k>>=1,a*=a%m,a%=m)
        if(k&1) ans*=a%m,ans%=m;
    return ans%m;
}
