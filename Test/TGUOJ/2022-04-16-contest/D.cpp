#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    int n;
    cin>>n;
    vector<int> nums(n);
    vector<int> dp;
    for (int i=0;i<n;i++) cin>>nums[i];
    dp.push_back(nums[0]);
    for (int i=1;i<n;i++) {
        bool flag=1;
        for (int j=0;j<dp.size();j++) {
            if (nums[i]>dp[j]){
                dp[j]=nums[i];
                flag=0;
                break;
            }
        }
        if (flag) dp.push_back(nums[i]);
    }
    cout<<dp.size()<<endl;
    system("pause");
    return 0;
}