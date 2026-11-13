/*
问题描述：
　　1221是一个非常特殊的数，它从左边读和从右边读是一样的，编程求所有这样的四位十进制数。
输出格式
　　按从小到大的顺序输出满足条件的四位十进制数。
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    vector<int> ve(2);
    ve.clear();
    for(int i=10;i<=99;i++)
    {
        string ans="";
        ve.push_back(i%10);
        ve.push_back(i/10%10);
        ans+=ve[1]+48;
        ans+=ve[0]+48;
        ans+=ve[0]+48;
        ans+=ve[1]+48;
        cout<<ans<<endl;
        ve.clear();
    }
    system("pause");
    return 0;
}