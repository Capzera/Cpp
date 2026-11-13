#include <bits/stdc++.h>

using namespace std;

bool isPrime(int n){
    int i,flag=1;
    for (i=2;i<sqrt(n)&&flag;i++) {
        if (n%i==0) flag=0;
    }
    return flag;
}

int main(){
    for (int i=100;i<=200;i++){
        if (isPrime(i)) cout<<i<<" ";
    }
    system("pause");
    return 0;
}