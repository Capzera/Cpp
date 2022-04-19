#include<bits/stdc++.h>
using namespace std;
int main(){
    int x1,x2,y1,y2;
    cin>>x1>>y1>>x2>>y2;
    x2=abs(x2-x1);
    y2=abs(y2-y1);
    x1=y1=0;
    if (x2>y2) swap(x2,y2);
    cout<<2*y2-((y2-x2)%2)<<endl;
    return 0;
}
/*
0 0
4 4
*/