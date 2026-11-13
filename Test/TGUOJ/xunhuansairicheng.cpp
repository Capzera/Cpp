#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,exp=1,wide=0;
    cin>>n;
    while (exp!=n){
        if (exp>n){
            cout<<"ERROR"<<endl;
            system("pause");
            return 0;
        }
        exp*=2;
    }
    vector<vector<int>> ans(n,vector<int>(n));
    ans[0][0]=1;
    while (wide<n)
    {
        for(int i=0;i<wide;i++){
            for(int j=0;j<wide;j++) 
            {
                ans[i+wide][j+wide]=ans[i][j];//对角线复制
                ans[i][j+wide]=ans[i+wide][j]=ans[i][j]+wide;//向下向右复制
            }
        }
        if (!wide) wide=1;
        else wide*=2;
    }
    for(auto i:ans){
        for(int j:i) cout<<j<<" ";
        cout<<endl;
    }
    system("pause");
    return 0;
}