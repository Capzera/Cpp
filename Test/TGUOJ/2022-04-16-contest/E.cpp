#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    int n,V,q,k=0;
    cin>>V>>n;
    vector<int> dp(V+1);
    vector<vector<int>> w(n,vector<int>(3)),v(n,vector<int>(3));
    for (int i=0;i<n;i++){
        cin>>w[i][0]>>v[i][0]>>q;
        for (int j=1;j<=q;j++) {
            cin>>w[i][j]>>v[i][j];
        }
    }
    for (int i=0;i<n;i++) {
        int flag=1;
        for (int j=V;j>=w[i][k];j--) {
            if (j-w[i][0]>=0) {
				dp[j] = max(dp[j],dp[j-w[i][0]]+v[i][0]);
				if (j-w[i][0]-w[i][1]>=0)
					dp[j] = max(dp[j],dp[j-w[i][0]-w[i][1]]+v[i][0]+v[i][1]);
				if (j-w[i][0]-w[i][2]>=0)
					dp[j] = max(dp[j],dp[j-w[i][0]-w[i][2]]+v[i][0]+v[i][2]);
				if (j-w[i][0]-w[i][1]-w[i][2]>=0)
					dp[j] = max(dp[j],dp[j-w[i][0]-w[i][1]-w[i][2]]+v[i][0]+v[i][1]+v[i][2]);
			}
        }
    }
    cout<<dp[V]<<endl;
    system("pause");
    return 0;
}