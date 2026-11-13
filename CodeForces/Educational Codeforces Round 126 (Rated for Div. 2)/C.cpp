#include<bits/stdc++.h>
using namespace std;
int main(){
    int t;
    cin>>t;
    while (t--){
        int n,maxh=-1,day=0,odd=0,even=0;
        cin>>n;
        vector<int> t(n);
        for (int i=0;i<n;i++) {
            cin>>t[i];
            maxh=max(maxh,t[i]);
        }
        priority_queue<int> q,tmp;
        for (int i=0;i<n;i++) {
            if (t[i]==maxh) continue;
            q.push(maxh-t[i]);
        }
        while (q.size()){
            day++;
            int x=q.top();q.pop();
            if (day%2){
                x--;
                q.push(x);
            }
            else {
                if (x>1) x-=2;
                q.push(x);
            }
        }
        cout<<day<<endl;
    }
    system("pause");
    return 0;
}