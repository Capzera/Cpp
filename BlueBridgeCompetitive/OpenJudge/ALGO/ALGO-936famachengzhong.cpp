/*
问题描述：
    小明捡到了一架没有游标的天平和N个标有重量的砝码，于是他想知道他能够称出多少种不同的重量（假设只能将砝码放在一侧）。
输入格式：
    输入的第一行包含一个正整数N，表示有N个砝码。接下来一行有N个正整数，表示N个砝码的重量。
输出格式：
　　输出一行，包含一个整数，表示能够称出多少种不同的重量。
样例输入：
    3
    1 2 3
样例输出：
    6
数据规模和约定
　　N<16,砝码重量<=1000。    
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int w[16],dp[16][16000],i,j,n,sum=0,ans=0;
    memset(w,0,sizeof(w));
    memset(dp,0,sizeof(dp));
    cin>>n;
    for(i=0;i<n;i++)
    {
        cin>>w[i];
        dp[i][w[i]]=1;
        sum+=w[i];
    }
    for(i=0;i<n;i++)
    {
        for(j=1;j<=sum;j++)
        {
            if (i>0&&dp[i-1][j])
            {
                dp[i][j]=1;
                dp[i][j+w[i]]=1;
            }
        }
    }
    for(i=1;i<=sum;i++)if(dp[n-1][i]) ans++;
    cout<<ans<<endl;
    system("pause");
    return 0;
}