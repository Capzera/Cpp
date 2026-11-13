#include<bits/stdc++.h>
using namespace std;
int main(){
    int t,a,b;
	cin>>t;
	for (int T=0;T<t;T++) {
		cin>>a>>b;
        if (a==0) cout<<1<<endl;
        else cout<<a+2*b+1<<endl;
	}
	system("pause");
    return 0;
}