#include<bits/stdc++.h>
using namespace std;
long long ai[200001],bi[200001];
bool lcm(long long l,long long r,long long j) {
	long long s;
	while(l<r) {
		s=(l+r+1)/2;
		if(bi[s]>ai[j]) {
			r=s-1;
		}
		if(bi[s]<=ai[j]) {
			l=s+1;
//			cout<<ai[j]<<endl;
			return 1;
		}
		return 0;
	}
}
int main() {
	long long a,b,l=0;
	cin>>a>>b;
	for(int i=1; i<=a; i++) {
		cin>>ai[i];
	}
	for(int i=1; i<=b; i++) {
		cin>>bi[i];
	}
	sort(ai+1,ai+a+1);
	sort(bi+1,bi+b+1);
	for(long long j=1; j<=b; j++) {
		if(lcm(1,b,j)==true) {
			l++;
		}
	}
	cout<<l;
	return 0;
}
