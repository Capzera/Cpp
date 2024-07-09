#include<iostream>
using namespace std;
int main(){
	int m,s=0,j=0,a=0;
	cin>>m;
	for(int i=1;a<m;i++){
		for(j=1;j<=i&&a<m;++j){
			s+=i;
			a++;
		} 
	}
	cout<<s;
	return 0;
}
