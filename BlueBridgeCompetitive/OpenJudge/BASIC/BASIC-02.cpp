/*
问题描述：
求1+2+3+...+n的值。
输入格式
输入包括一个整数n。
输出格式
输出一行，包括一个整数，表示1+2+3+...+n的值。
样例输入
4
样例输出
10
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    long long n,ans=0,sum=1;
    cin>>n;
    sum=(n+1)*n/2;
    ans+=sum;
    printf("%I64d",ans);
    system("pause");
    return 0;
}