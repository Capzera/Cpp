#include<bits/stdc++.h>
using namespace std;
long long t,n,m,k,ans1,sum;
long long a[1005],b[1005];
int main(){
	//freopen(" .in","r",stdin);
	//freopen(" .out","w",stdout);
	cin>>t;
	while(t--){
		ans1=0;
		cin>>n>>m>>k;
		for(int i=0;i<n;i++){
			cin>>a[i];
		}
		sort(a,a+n);
		for(int i=0;i<m;i++){
			cin>>b[i];
		}
		sort(b,b+m);
		swap(a[0],b[m-1]);
		sort(a,a+n);
		sort(b,b+m);
		if(k%2==0){
			swap(b[0],a[n-1]);
		}
		for(int i=0;i<n;i++){
			ans1+=a[i];
		}
		cout<<ans1<<endl;
	}
    system("pause");
	return 0;
}