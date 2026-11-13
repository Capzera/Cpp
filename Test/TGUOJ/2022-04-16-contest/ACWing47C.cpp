#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<int> nums(n+2),ans;
    for (int i=1;i<=n;i++) cin>>nums[i];
    for (int i=n;i>1;i--) nums[i]-=nums[i-1];
    for (int i=1;i<=n;i++) {
        bool check=1;
        for (int j=i+1;j<=n;j++) {
            if (nums[j]!=nums[j-i]){
                check=0;
                break;
            }
        }
        if (check) ans.push_back(i);
    }
    n=ans.size();
    cout<<n<<endl;
    for (int i=0;i<n;i++) cout<<ans[i]<<" ";
    system("pause");
    return 0;
}