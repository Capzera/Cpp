#include<bits/stdc++.h>
using namespace std;
long long n,m,a,k,b,d;
int main(){
	cin>>n>>a>>k>>b;
	if(n%2){
		cout<<"-1";
		return 0;
	}
	m=n/2;
	if(a*k>b){
		d+=m/k*b;
		if(m%k*a>b){
			d+=b;
		}
		else{
			d+=m%k*a;
		}
	}
	else{
		d=m*a;
	}
	cout<<d*2;
	return 0;
}
