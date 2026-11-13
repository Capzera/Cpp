#include<bits/stdc++.h>
using namespace std;
int ans=0,n,k;
vector<int> nums,sum;
    void dfs(int i,int cur) {
        if (cur>k) return;
        if (cur==k||i==n-1) {
            ans++;
            if (ans==998244353) ans=0;
            return;
        }
        for (int j=min(nums[i],k-cur);j>=max(k-(sum[n]-sum[i+1]+cur),0);j--) {
            dfs(i+1,cur+j);
        }
    }
int main(){
    cin>>n>>k;
    nums=vector<int> (n);
    sum=vector<int> (n+1);
    for (int i=0;i<n;i++) {
        cin>>nums[i];
        sum[i+1]=sum[i]+nums[i];
    }
    sort(nums.begin(),nums.end());
    dfs(0,0);
    cout<<ans<<endl;
    return 0;
}