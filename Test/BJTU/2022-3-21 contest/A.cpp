#include<bits/stdc++.h>
using namespace std;
int main(){
    long long n,ans=0;
    int m;
    cin>>n;
    m=sqrt(n);
    for (int i=1;i<=m;i++) {
        long long tmp=i;
        tmp*=i;
        long long x=(n-tmp)/2;
        if (2*x+tmp==n&&x>0) ans++;
    }
    cout<<ans<<endl;
    return 0;
}