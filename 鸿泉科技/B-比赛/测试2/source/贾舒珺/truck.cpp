#include<iostream>
using namespace std;
int main(){
	long long a,b,x,k;
	cin>>a>>b>>x>>k;
	if((a-1)/(k-1)>b) cout<<(a+b)*x;
	else cout<<(a+(a-1)/(k-1))*x;
	return 0;
}
