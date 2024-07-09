#include<iostream>
#include<algorithm>
using namespace std;
const int N=200011;
int main(){
	int n,m,a[N],b[N];
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	for(int i=1;i<=m;i++){
		cin>>b[i];
	}
	sort(a+1,a+1+n);
	sort(b+1,b+1+m);
	int x=1,ans=0;
	for(int i=1;i<=n;i++){
		if(a[i]>b[x]){
			ans++;
			x++;
		}
	}
	cout<<ans;
	return 0;
}
