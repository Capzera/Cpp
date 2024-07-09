#include<bits/stdc++.h>
using namespace std;
int main(){
	int a,n,k,b;
	cin>>n>>a>>k>>b;
	if(n%2!=0){
		cout<<"-1";
	}
	else{
		n/=2;
		if(n%k==0){
			if(n/k*b<n*a){
                cout<<n/k*b*2;	
			}
			else{
				cout<<n*a;
			}
		}
		else{
			if(n/k*b+b<n*a){
				cout<<(n/k*b+b)*2;
			}
			else{
				cout<<n*a;
			} 
		}
	}
	return 0;
}
