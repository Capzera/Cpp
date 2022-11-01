#include<bits/stdc++.h>
using namespace std;
static bool cmp(const vector<int>& a,const vector<int>& b) {
    return a[0]*a[1]<b[0]*b[1];
}
int main(){
    int n,kl,kr,ans;
    cin>>n;
    cin>>kl>>kr;
    vector<vector<int>> val(n,vector<int>(2));
    for (int i=0;i<n;i++) cin>>val[i][0]>>val[i][1];
    sort(val.begin(),val.end(),cmp);
    for (int i=0;i<n;i++){
        ans=max(ans,kl/val[i][1]);
        kl*=val[i][0];
    }
    cout<<ans<<endl;
    system("pause");
    return 0;
}