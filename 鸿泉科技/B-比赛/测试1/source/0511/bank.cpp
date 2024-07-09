#include<bits/stdc++.h>
using namespace std;
int main(){
	int m,s=0;
	cin >> m;
	for(int i=1,j=1,n=1;i<=m;i++){
		s=s+j;
		if(n==j){
			j++;
			n=1;
		}else{
			n++;
		}
	}
	cout << s;
	return 0;
}

