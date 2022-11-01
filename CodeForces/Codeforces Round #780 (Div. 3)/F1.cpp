#include<bits/stdc++.h>
using namespace std;
int main(){
    int T,n,ans;
    string s;
    cin>>T;
    while (T--){
        cin>>n;cin>>s;
        ans=0;
        vector<int> sum(n+1);
        for (int i=1;i<=n;i++){
            sum[i]=sum[i-1];
            if (s[i-1]=='+') sum[i]++;
        }
        for (int i=0;i<n-1;i++) {
            for (int j=i+1;j<n;j++) {
                int plus=sum[j+1]-sum[i];
                int minus=j-i+1-plus;
                if (minus==plus||((minus-plus)%3==0&&minus>plus)) ans++;
            }
        }
        cout<<ans<<endl;
    }
    system("pause");
    return 0;
}