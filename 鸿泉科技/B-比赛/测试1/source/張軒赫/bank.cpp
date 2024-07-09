#include<bits/stdc++.h>
using namespace std;
long long a,b,x,k,d;
int main(){
	cin>>a>>b>>x>>k;
	while(a!=0){
		d+=a;
		if(b>=a/k){
			a=a/k;
		}
	}
	cout<<d*x;
	return 0;
}
