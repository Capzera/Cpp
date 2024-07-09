#include<bits/stdc++.h>
using namespace std;
long long n,a[600000],ans; 
int main(){
	cin>>n;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}	
	sort(a+1,a+1+n);
	ans=a[1];
	for(int i=2;i<=n;i++){
		a[i]=a[i]-ans;
		ans+=a[i];
	}
	cout<<a[n];
	
    return 0; 
}
