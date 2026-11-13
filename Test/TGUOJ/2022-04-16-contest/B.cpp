#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    int n,m;
    cin>>n>>m;
    vector<int> dp(n+1,1000000);
    dp[1]=0;
    for (int i=1;i<=n;i++) {
        for (int j=min(n,i+m);j>i;j--) {
            dp[j]=min(dp[j],__gcd(i,j)+dp[i]);
        }
    }
    cout<<dp[n]<<endl;
    system("pause");
    return 0;
}