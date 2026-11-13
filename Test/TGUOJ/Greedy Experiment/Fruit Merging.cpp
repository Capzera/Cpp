#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,put,ans=0;
    cin>>n;
    priority_queue<int,vector<int>,greater<int>> q;
    for (int i=0;i<n;i++) {
        cin>>put;
        q.push(put);
    }
    while (q.size()>1){
        int top1=q.top();
        q.pop();
        int top2=top1+q.top();
        ans+=top2;
        q.pop();
        q.push(top2);
    }
    cout<<ans<<endl;
    system("pause");
    return 0;
}