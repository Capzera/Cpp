#include<bits/stdc++.h>
using namespace std;
int main(){
    int t;
    cin>>t;
    for (int T=0;T<t;T++) {
        int n,ans=0;
        string s;
        cin>>n;
        vector<int> row(n),col(n);
        for (int i=0;i<n;i++) {
            cin>>s;
            for (int j=0;j<s.size();j++)
                if (s[j]=='1') {
                    row[i]++;
                    column[j]++;
                }
        }
        for (int i=0;i<n;i++) {
            if (row[i]==col[i]) {
                if(row[i]>1) ans+=row[i]-1;
                else if (!row[i]) ans++;
            }
            else {

            }
        }
        cout<<ans<<endl;
    }
    system("pause");
    return 0;
}

1 1 1 0
1 0 0 0
0 1 0 0
0 0 0 1



3 1 1 1
2 2 1 1
3+1


1 1 1 0
1 0 0 0
0 1 0 0
0 0 0 1
2 1 1
2 1 1


1 0 0
0 0 1
0 1 0