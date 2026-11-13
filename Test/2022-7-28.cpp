#include<bits/stdc++.h>

using namespace std;

int main(){
    int n=20;
    vector<int> dp;
    dp.push_back(1);
    cout<<dp.back()<<endl;
    for(int i=1;i<=n;i++) {
        dp.push_back(1);
        for (int j=dp.size()-2;j>=1;j--)
            dp[j]+=dp[j-1];
        for (int j=0;j<dp.size();j++) cout<<dp[j]<<" ";
        cout<<endl;
    }
    system("pause");
    return 0;
}