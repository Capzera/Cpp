#include <bits/stdc++.h>

using namespace std;

int bitc(int n){
    int ans=0;
    while (n){
        if (n%2) ans++;
        n/=2;
    }
    return ans;
}

int main(){
    int n;
    cin>>n;
    cout<<bitc(n)<<endl;
    system("pause");
    return 0;
}