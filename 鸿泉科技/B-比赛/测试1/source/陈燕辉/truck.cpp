#include<bits/stdc++.h>
using namespace std;
int main() {
	long long a,b,c,k,p=0;
	cin>>a>>b>>c>>k;
	while(a>0){
		if(a-k>=0){
			a++;
			a-=k;
			p+=k*c;
		}else{
			a--;
			p+=1*c;
		}
	}
	cout<<p;
	return 0;
}
