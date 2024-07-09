#include<iostream>
using namespace std;
int main(){
	int a,b,x,k,m,s=0;
	cin>>a>>b>>x>>k;
	for(;a>0;a--){
		s+=x;
		m++;
		if(m==k&&b!=0){
			m=0;
			b-=1;
			a++;
		}
	} 
	cout<<s;
	return 0;
}