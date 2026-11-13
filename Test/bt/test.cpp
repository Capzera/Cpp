#include<bits/stdc++.h>

using namespace std;
int cnt=0;
int fib(int n){
    cnt++;
    if (!n) return 1;
    if (n==1) return 2;
    return fib(n-1)+fib(n-2);
}
int main(){
    fib(8);
    cout<<cnt<<endl;
    system("pause");
    return 0;
}