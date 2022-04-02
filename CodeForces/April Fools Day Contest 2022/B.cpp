#include <bits/stdc++.h>
using namespace std;
int main() {
    int N,t;
    set<int> f;
    for (N=1;N<=52;++N) {
    int G=0, i, j;
    for(i=1; i<N; i++)
        for(j=i+1; j<=N; j++) {
            G+=__gcd(i, j);
        }
    f.insert(G);
    }
    cin>>t;
    if (f.find(t)==f.end()) cout<<t+1<<endl;
    else cout<<t+2<<endl;
    system("pause");
    return 0;
}