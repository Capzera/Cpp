#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
	vector<int> w(n),v(n);
    vector<int> dp(m+1);
    for (int i=0;i<n;i++) cin>>w[i];
    sort(w.begin(),w.end());
    for (int i=0;i<n;i++) {
        v[i]=w[i]*(i+1);
        w[i]=(i+1)*(i+1);
    }
	for(int i=0;i<n;i++)
        for(int j=m;j>=0;j--) {
            if (w[i]==j) dp[j]=v[i];
            if (w[i]<j) dp[j]=max(dp[j],dp[j-w[i]]+v[i]);
        }
    cout<<dp[m]<<endl;
    return 0;
}