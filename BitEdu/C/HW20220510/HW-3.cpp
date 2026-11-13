#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin>>n;
    int a[n],b[n],c[n];
    for (int i=0;i<n;i++) cin>>a[i];
    for (int i=0;i<n;i++) cin>>b[i];
    for (int i=0;i<n;i++) swap(a[i],b[i]);
    cout<<"A[]:"<<endl;
    for (auto i:a) cout<<i<<" ";
    cout<<endl<<"B[]:"<<endl;
    for (auto i:b) cout<<i<<" ";
    system("pause");
    return 0;
}