#include <bits/stdc++.h>
using namespace std;
int main () {
	int a,b,c,d,n;
	cin>>n;
	if(n%2>0&&n%3==0){
		cout<<"1 1 0 0";
	}
	else{
		if(n%2>0||n%3==0){
			cout<<"0 1 1 0";
		}
		else{
			cout<<"0 0 0 1";
		} 
	}
return 0;
}
