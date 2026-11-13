#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,ans=0;
    cin>>n;
    vector<vector<int>> grid(n,vector<int>(n));
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) cin>>grid[i][j];
    }
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++)
            if (grid[i][j]!=grid[j][i]) ans++;
    }
    cout<<ans/2<<endl;
    return 0;
}
/*
3
1 2 4
2 3 6
3 7 9
*/