#include<bits/stdc++.h>
using namespace std;
    long long pow_mod(int b){
    long long ans=1;
    int a=10,c=1000000007;
    int base=a%c;
    while(b){
        if(b & 1) ans = (ans*base)%c;
        base = (base*base)%c;
        b >>= 1;
    }
    return ans%c;
}
int main(){
    long long n,ans=0,tmp=9,exp=1;
    cin>>n;
    if (n==1) ans=9;
    if (n==2) ans=18;
    if (n>2){
        ans=18;
        n-=2;
        while (n){
            tmp+=pow_mod(exp++);
            ans=(ans%1000000007+tmp%1000000007)%1000000007;
            n--;
        }
    }   
    cout<<ans<<endl;
    return 0;
}