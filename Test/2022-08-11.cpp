#include<bits/stdc++.h>

using namespace std;

int main(){
    int i=0;
    int *p=&i;
    int cnt=0;
    while (p-- && p[++i]){
        cnt++;
        //if (cnt==100) break;
    }
    cout<<cnt<<endl;
    system("pause");
    return 0;
}