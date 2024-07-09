#include<iostream>
using namespace std;
long long a,b,x,k; 
int main(){
	cin>>a>>b>>x>>k;
	cout<<a*x+min(a/k,b)*x;
	return 0;
}
