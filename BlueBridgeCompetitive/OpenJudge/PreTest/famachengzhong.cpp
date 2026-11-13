/*
问题描述：
    你有一架天平和N个砝码，这N个砝码重量依次是W1,W2,…,WN.
    请你计算一共可以称出多少种不同的重量?
    注意砝码可以放在天平两边
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int w[101],dp[101][100001],i,j,n,sum=0,ans=0,min=100001;
    memset(w,0,sizeof(w));
    memset(dp,0,sizeof(dp));
    cin>>n;
    for(i=0;i<n;i++)
    {
        cin>>w[i];
        dp[i][w[i]]=1;
        sum+=w[i];
        if (w[i]<min) min=w[i];
    }
    for(i=0;i<n;i++)
    {
        for(j=min;j<=sum;j++)
        {
            if (i>0&&dp[i-1][j])
            {
                dp[i][j]=1;
                dp[i][j+w[i]]=1;
                dp[i][abs(j-w[i])]=1;
            }
        }
    }
    for(i=1;i<=sum;i++)if(dp[n-1][i]) ans++;
    cout<<ans<<endl;
    system("pause");
    return 0;
}