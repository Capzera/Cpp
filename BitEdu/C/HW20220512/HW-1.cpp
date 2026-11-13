#include <bits/stdc++.h>

using namespace std;

int main(){
    int m,n,ans=0;
    cin>>m>>n;
    string s1,s2;
    while (m){
        s1+=to_string(m%2);
        m/=2;
    }
    while (n){
        s2+=to_string(n%2);
        n/=2;
    }
    if (s1.size()<s2.size()) swap(s1,s2);
    for (int i=0;i<s1.size();i++) {
        if (i>=s2.size()&&s1[i]=='1') ans++;
        else if (i<s2.size()&&s1[i]!=s2[i]) ans++;
    }
    cout<<ans<<endl;
    system("pause");
    return 0;
}