#include<bits/stdc++.h>
using namespace std;
int main(){
    int t;
    cin>>t;
    while (t--){
        string s;
        cin>>s;
        int n=s.size();
        vector<int> dp(n+1),alpha(26,-1);
        for (int i=1;i<=n;i++){
            int j=s[i-1]-97;
            dp[i]=dp[i-1];
            if (alpha[j]>=0) dp[i]=max(dp[i],dp[alpha[j]]+2);
            alpha[j]=i-1;
        }
        cout<<n-dp[n]<<endl;
    }
    system("pause");
    return 0;
}