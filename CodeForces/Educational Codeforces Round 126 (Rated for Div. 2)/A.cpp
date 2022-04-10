#include<bits/stdc++.h>
using namespace std;
int main(){
    int t;
    cin>>t;
    while (t--){
        int n;
        cin>>n;
        vector<int> a(n),b(n),dp(n,0);
        for (int i=0;i<n;i++) cin>>a[i];
        for (int i=0;i<n;i++) cin>>b[i];
        for (int i=1;i<n;i++) {
            dp[i]+=dp[i-1]+min(abs(a[i]-a[i-1])+abs(b[i]-b[i-1]),abs(a[i]-b[i-1])+abs(b[i]-a[i-1]));
        }
        cout<<dp[n-1]<<endl;
    }
    system("pause");
    return 0;
}