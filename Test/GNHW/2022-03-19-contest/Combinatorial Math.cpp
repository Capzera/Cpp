#include<bits/stdc++.h>
using namespace std;
    int n;
    void dfs(int i,vector<int> ans,vector<bool> check){
        if (i==n){
            for(int t:ans) cout<<t<<" ";
            cout<<endl;
            return;
        }
        for(int j=0;j<n;j++) {
            if (check[j]) continue;
            check[j]=1;
            ans[i]=j+1;
            dfs(i+1,ans,check);
            check[j]=0;
        }
    }
int main(){
    cin>>n;
    vector<int> ans(n);
    vector<bool>check(n);
    dfs(0,ans,check);
    system("pause");
    return 0;
}