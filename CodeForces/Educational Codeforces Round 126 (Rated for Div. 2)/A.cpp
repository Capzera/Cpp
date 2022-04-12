#include<bits/stdc++.h>
using namespace std;
int main(){
    int t;
    cin>>t;
    while (t--){
        int n;
        cin>>n;
        vector<int> a(n),b(n);
        for (int i=0;i<n;i++) cin>>a[i];
        for (int i=0;i<n;i++) cin>>b[i];
        for (int i=0;i<n-1;i++) {
            dp+=min(abs(a[i]-a[i+1])+abs(b[i]-b[i+1]),abs(a[i]-b[i+1])+abs(b[i]-a[i+1]));
        }
        cout<<dp<<endl;
    }
    system("pause");
    return 0;
}