#include<iostream>
#include<algorithm>
using namespace std;
long long n,m,ans,a[300030],b[300030]; 
int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++) cin>>a[i];
	for(int j=1;j<=m;j++) cin>>b[j]; 
	sort(a+1,a+1+n);
	sort(b+1,b+1+m); 
	int j=1;
	for(int i=1;i<=m;i++){
		if(a[i]>b[j]){
			ans++;
			j++;
		}
	}
	cout<<ans;
	return 0;
}
