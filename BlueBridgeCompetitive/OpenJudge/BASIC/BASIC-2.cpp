/*
问题描述：
对于长度为5位的一个01串，每一位都可能是0或1，一共有32种可能。它们的前几个是：
00000
00001
00010
00011
00100
请按从小到大的顺序输出这32种01串。
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    for(int i=0;i<32;i++)
    {
        string ans="";
        ans.push_back(i%2+48);
        ans.push_back(i/2%2+48);
        ans.push_back(i/2/2%2+48);
        ans.push_back(i/2/2/2%2+48);
        ans.push_back(i/2/2/2/2%2+48);
        reverse(ans.begin(),ans.end());
        cout<<ans<<endl;
    }
    system("pause");
    return 0;
}