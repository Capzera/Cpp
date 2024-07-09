#include<bits/stdc++.h>
using namespace std;
long long n,m,a[100010],sum,c[100010],x=0,b=0,flag=0;
int main(){
	cin.tie(nullptr)->sync_with_stdio(0);
	freopen("work.in","r",stdin);
	freopen("work.out","w",stdout);
	cin>>n>>m;
	for(int i=0;i<n;i++){
		cin>>a[i];
		sum+=a[i];
		c[i]=a[i];
	}
	x=n;
	sort(c,c+n);
	if(sum/n>=m){
		cout<<n;
	}
	else{
		for(int i=0;i<n;i++){
			sum-=c[i];
			x--;
			if(sum>=m*x){
				cout<<x;
				break;
			}
		}
	}
	return 0;
}
