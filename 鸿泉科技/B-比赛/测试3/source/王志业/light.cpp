#include<bits/stdc++.h>
using namespace std;
#define ll long long
ll n,a[9999999]={0,2};
int main() {
	cin>>n;
	for(ll i=2;i<=n;i++){
		a[i]=a[i-1]+i-1;
	}
	cout<<a[n];
	return 0;
}
//1 2
//2 3
//3 5
//4 8
//5 12
//6 17
