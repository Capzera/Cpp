#include<bits/stdc++.h>
using namespace std;
int main() {
	long long n,a,b,c,che,ban,l=0;
	cin>>n>>c>>che>>ban;
	if(n%2==1) {
		cout<<-1;
		return 0;
	} else {
		a=b=n/2;
	}
	c--;
	if(che/c<ban){
		if(a/c==0){
			l+=a/c*che; 
		}else{
			l+=min(a/c*che+a%c*ban,(a/c+1)*che);
		}
		if(b/c==0){
			l+=b/c*che; 
		}else{
			l+=min(b/c*che+b%c*ban,(b/c+1)*che);
		}
	}
	cout<<l;
	return 0;
}
