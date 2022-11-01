#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
	vector<int> w(n+1);
    vector<int> dp(m+1);
    for (int i=0;i<n;i++) cin>>w[i];
	for(int i=0;i<n;i++)
        for(int j=m;j>=0;j--) {
            if (w[i]==j) dp[j]=1;
            if (w[i]<j) dp[j]=max(dp[j],dp[j-w[i]]+1);
        }
    cout<<dp[m]<<endl;
    return 0;
}