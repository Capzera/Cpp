#include<bits/stdc++.h>
using namespace std;
int main(){
    string s,ans="";
    cin>>s;
    for (auto i:s) {
        int x=i;
        ans+=to_string(x);
    }
    cout<<ans<<endl;
    return 0;
}