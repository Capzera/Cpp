#include<bits/stdc++.h>

using namespace std;

vector<int> nums;
vector<bool> index;

int n;

set<int> ans;

void dfs(int i,int sum){
    if (i==n) {
        ans.insert(sum);
        return;
    }
    for (int j=i;j<n;j++) {
        if (index[j]) continue;
        index[j]=1;
        dfs(i+1,sum+nums[j]);
        dfs(i+1,sum-nums[j]);
        dfs(i+1,sum);
        index[j]=0;
    }
}

int main(){
    cin>>n;
    nums=vector<int> (n);
    index=vector<bool> (n);
    for (int i=0;i<n;i++) cin>>nums[i];
    dfs(0,0);
    cout<<ans.size()<<endl;
    system("pause");
    return 0;
}