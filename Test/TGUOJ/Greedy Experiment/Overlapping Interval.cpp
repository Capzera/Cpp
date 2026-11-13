#include<bits/stdc++.h>
using namespace std;
static bool cmp(const vector<int>& a,const vector<int>& b){
    return a[1]<b[1];
}
int main(){
    int n;
    cin>>n;
    vector<vector<int>> intervals(n,vector<int>(2));
    for (int i=0;i<n;i++) cin>>intervals[i][0]>>intervals[i][1];
    sort(intervals.begin(),intervals.end(),cmp);
    int ans=0,right=intervals[0][1];
    for(int i=1;i<n;i++) {
        if (intervals[i][0]<right) ans++;
        else right=intervals[i][1];
    }
    cout<<n-ans<<endl;
    system("pause");
    return 0;
}