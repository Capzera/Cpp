#include<bits/stdc++.h>
using namespace std;
union{
	char ch[8];
    double k;
    int t;
}u;

int main(){
    u.ch[5]=2;
    u.ch[6]=0;
    cout<<u.k<<" "<<u.t<<endl;
    system("pause");
    return 0;
}