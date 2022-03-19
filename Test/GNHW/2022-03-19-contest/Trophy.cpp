#include<bits/stdc++.h>a
using namespace std;
int main(){
    int n,q,put;
    cin>>n>>q;
    unordered_map<int,int> um;
    for(int i=0;i<n;i++) {
        cin>>put;
        um[put]++;
    }
    for(int i=0;i<q;i++) {
        cin>>put;
        if (um.count(put)) cout<<um[put]<<endl;
        else cout<<-1<<endl;
    }
    system("pause");
    return 0;
}