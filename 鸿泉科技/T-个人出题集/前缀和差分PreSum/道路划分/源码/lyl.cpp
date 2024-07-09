#include<bits/stdc++.h> 
using namespace std;
int n, ans = INT_MIN,x;
int a[200010];
int main(){
	cin>>n;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	sort(a+1,a+n+1);
	ans=a[1];
	for(long long i=1;i<n;i++){
		for(long long j=i+1;j<=n;j++){
			a[j]-=a[i];
		}
		ans=max(ans,a[i+1]);
	}
	cout<<ans<<endl;
	return 0;
}
