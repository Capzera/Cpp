#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,wide=1,maxdepth=0;
    char origin;
    cin>>n>>origin;
    while (1) {
        if (n-wide>=0) {
            maxdepth++;
            n-=wide;
            wide+=2;
        }
        else break;
    }
    for(int i=0;i<maxdepth;i++) {
        for(int j=0;j<maxdepth-i-1;j++) cout<<" ";
        for(int j=0;j<2*i+1;j++) cout<<origin;
        cout<<endl;
    }
    cout<<n<<endl;
    system("pause");
	return 0; 
}