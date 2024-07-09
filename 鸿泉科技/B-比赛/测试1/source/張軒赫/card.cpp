#include<bits/stdc++.h>
using namespace std;
long long n,m,a[1000001],b[1000001],k;
int nb(int k){
	int left=1,right=n,mid,f=0;
	while(left<right){
		mid=(left+right)/2;
		if(a[mid]<=k){
			left=mid+1;
		}
		else{
			right=mid;
		}
	}
	return left;
}
void nb2(int k){
	for(int i=k;i<n;i++){
		a[i]=a[i+1];
	}
	n--;
}
int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	for(int i=1;i<=m;i++){
		cin>>b[i];
	}
	sort(a+1,a+n+1);
	sort(b+1,b+m+1);
	for(int i=1;i<=m;i++){
		if(a[n]<=b[i]){
			break;
		}
		k++;
		nb2(nb(b[i]));
	}
	cout<<k;
	return 0;
}
