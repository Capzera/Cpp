/*
问题描述：
给定一个年份，判断这一年是不是闰年。
当以下情况之一满足时，这一年是闰年：
1. 年份是4的倍数而不是100的倍数；
2. 年份是400的倍数。
其他的年份都不是闰年。
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int year;
    cin>>year;
    if ((year%4==0&&year%100!=0)||year%400==0) cout<<"yes"<<endl;
    else cout<<"no"<<endl;
    system("pause");
    return 0;
}