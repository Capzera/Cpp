/*
问题描述：
杨辉三角形又称Pascal三角形，它的第i+1行是(a+b)i的展开式的系数。　
它的一个重要性质是：三角形中的每个数字等于它两肩上的数字相加。　
下面给出了杨辉三角形的前4行：
   1　
  1 1　
 1 2 1
1 3 3 1
给出n，输出它的前n行。
输入格式
输入包含一个数n。
输出格式
输出杨辉三角形的前n行。每一行从这一行的第一个数开始依次输出，中间使用一个空格分隔。请不要在前面输出多余的空格。
样例输入
4
样例输出
1
1 1
1 2 1
1 3 3 1
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int> nums(n+1);nums.clear();
    for(int i=1;i<=n;i++)
    {
        nums[i-1]=1;
        for(int j=i-2;j>=1;j--) nums[j]+=nums[j-1];
        for(int j=0;j<i;j++) cout<<nums[j]<<" ";
        cout<<endl;
    }
    system("pause");
    return 0;
}