#include<iostream>
#include<algorithm>
using namespace std;
const int N=200011;
int a[N];
long long qzh[N],ans;
int main() {
	int n;
	cin>>n;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	sort(a+1,a+1+n);
	for(int i=1;i<=n;i++){
		qzh[i]=qzh[i-1]+a[i];
	}
	int z,g=0;
	ans=a[1];
	for(int i=2;i<=n;i++){
		z=(a[i]-g)-(qzh[i-1]-g*(i-1));
		if(z>ans){
			ans=z;
			g+=a[i-1];
		}
	}
	cout<<ans;
	return 0;
}
