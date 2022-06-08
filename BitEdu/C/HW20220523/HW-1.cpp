#include<bits/stdc++.h>
using namespace std;
int main(){
    string s,tmp="";
    vector<string> ans;
    int i;
    getline(cin,s);
    for (i=0;i<=s.size();i++){
        if (s[i]==' ') {
            ans.push_back(tmp);
            tmp="";
            continue;
        }
        tmp+=s[i];
    }
    if (tmp!="") {
        ans.push_back(tmp);
    }
    for (i=ans.size()-1;i>=0;i--) cout<<ans[i]<<" ";
    system("pause");
    return 0;
}