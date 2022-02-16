/*
问题描述：
Fibonacci数列的递推公式为：Fn=Fn-1+Fn-2，其中F1=F2=1。
当n比较大时，Fn也非常大，现在我们想知道，Fn除以10007的余数是多少。
输入格式
输入包含一个整数n。
输出格式
输出一行，包含一个整数，表示Fn除以10007的余数。
样例输入
10
样例输出
55
样例输入
22
样例输出
7704
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    if (n==1) cout<<1;
    else if (n==2) cout<<1;
    else
    {
        int x=1,y=1,sum=0;
        for(int i=3;i<=n;i++)
        {
            sum=(x+y)%10007;
            x=y;
            y=sum;
        }
        cout<<sum;
    }
    system("pause");
    return 0;
}