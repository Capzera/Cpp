#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,V,ans=-1,i,j;
    cin>>n>>V;
    vector<int> dp(V+1,INT_MIN),w(n),v(n);
    for (int i=0;i<n;i++) {
        cin>>w[i]>>v[i];
    }
    dp[0]=0;
    for (i=0;i<n;i++) {
        for (j=V;j>=w[i];j--) {
            dp[j]=max(dp[j],dp[j-w[i]]+v[i]);
            if (dp[j]<0) dp[j]=INT_MIN;
        }
    }
    if (dp[V]<0) cout<<0<<endl;
    else cout<<dp[V]<<endl;
    system("pause");
    return 0;
}