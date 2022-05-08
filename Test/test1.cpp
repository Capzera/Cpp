#include<bits/stdc++.h>
using namespace std;
int gcd(int m,int n){
	if (m%n==0) return n;
	return gcd(n,m%n);
}
int main(){
	int a,b;
	cin>>a>>b;
	if (a<b) swap(a,b);
	cout<<gcd(a,b)<<endl;
	system("pause");
	return 0;
}