#include <bits/stdc++.h>

using namespace std;

bool isLeap(int n) {
    return (n%4==0&&n%100!=0)||(n%400==0);
}

int main(){
    int y;
    cin>>y;
    if (isLeap(y)) cout<<"YES"<<endl;
    else cout<<"No"<<endl;
    system("pause");
    return 0;
}