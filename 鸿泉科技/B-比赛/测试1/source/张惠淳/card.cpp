#include<bits/stdc++.h>
using namespace std;
int main(){
	int n,m,a[1000],b[1000],s=0;
	cin>>n>>m;
	for(int i=0;i<n;i++){
		cin>>a[i];
	}
	for(int i=0;i<m;i++){
		cin>>b[i];
	}
	sort(a,a+n);
	sort(b,b+m);
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			if(b[i]<a[j]){
				cout<<b[i]<<" "<<a[j]<<endl;
				s++;
				a[j]=0;
				b[i]=11451;
			}
		}
	}
	cout<<s;
	return 0;
}