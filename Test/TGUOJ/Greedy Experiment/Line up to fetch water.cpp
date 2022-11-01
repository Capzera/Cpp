#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<int> water(n);
    for (int i=0;i<n;i++) cin>>water[i];
    sort(water.begin(),water.end());
    int sum=water[0];
    double ans=0;
    for (int i=1;i<n;i++){
        ans+=sum;
        sum+=water[i];
    }
    ans/=n;
    cout<<setiosflags(ios::fixed)<<setprecision(2)<<ans<<endl;
    system("pause");
    return 0;
}