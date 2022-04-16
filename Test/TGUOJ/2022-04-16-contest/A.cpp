#include<bits/stdc++.h>
using namespace std;
int n,dp;
int main(){
    ios::sync_with_stdio(false);
    int ans=0;
    cin>>n;
    vector<int> nums(n);
    for (int i=0;i<n;i++) cin>>nums[i];
    dp=nums[0];
    for (int i=1;i<n;i++)
        dp+=max(0,nums[i]-nums[i-1]);
    cout<<dp<<endl;
    system("pause");
    return 0;
}