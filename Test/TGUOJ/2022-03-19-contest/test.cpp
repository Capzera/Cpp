#include<bits/stdc++.h>
using namespace std;
    int count=0,m;
    vector<int> ans(10);
    vector<bool>check(10);
    
    void dfs(int i,vector<int> ans,vector<bool> check){
      	if (count==m) return;
        if (i==10){
          	count++;
          	if (count==m){
            	for(int t:ans) cout<<t<<" ";
            	cout<<endl;
            }
            return; 
        }
        for(int j=0;j<10;j++) {
            if (check[j]) continue;
            check[j]=1;
            ans[i]=j;
            dfs(i+1,ans,check);
            check[j]=0;
        }
    }
int main(){
  	int m;
    cin>>m;
    dfs(0,ans,check);
    system("pause");
    return 0;
}