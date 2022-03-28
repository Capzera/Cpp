/*输入：背包容量 物品个数
每行为  重量 价值
*/
#include<bits/stdc++.h>
using namespace std;
int main() {
    int t,n;
    cin>>t>>n;
    vector<int> v(n),w(n),dp(t+1);
    for (int i=0;i<n;i++) cin>>w[i]>>v[i];
    for (int i=0;i<n;i++) {
        for (int j=t;j>=w[i];j--) {
            dp[j]=max(dp[j],dp[j-w[i]]+v[i]);
        }
    }
    cout<<dp[t]<<endl;
    return 0;
}