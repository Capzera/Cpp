#include<bits/stdc++.h>
using namespace std;
int main() {
	int n,k=0,p=0,i=0;
	cin>>n;
	while(i<n) {
		int o=1;
		for(int j=p;o<=p;o++) {
			if(i<n){
			k+=j;
//			cout<<j<<endl;
			i++;
			}
		}
		p++;
	}
	cout<<k;
	return 0;
}
