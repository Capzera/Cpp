#include<bits/stdc++.h>
using namespace std;
int main(){
    int ans=0,n;
    cin>>n;
    vector<int> nums(n);
    for (int i=0;i<n;i++) cin>>nums[i];
    int dp=nums[0];
    for (int i=1;i<n;i++)
        dp+=max(0,nums[i]-nums[i-1]);
    cout<<dp<<endl;
    system("pause");
    return 0;
}