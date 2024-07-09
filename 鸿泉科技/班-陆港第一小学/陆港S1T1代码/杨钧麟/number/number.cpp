#include<iostream>
using namespace std;
int main() {
	int n;
	cin>>n;
	if(n%2==0&&n%3==0) {
		cout<<0 ;
	} else
		cout<<1 ;
	if (n%3==0||n%2==0)
		cout<<1 ;
	else
		cout<<0 ;
	if (n%2==0||n%3==0) {
		cout<<0 ;
	} else
		cout<<1 ;
	if(n%2==0&&n%3==0) {
		cout<<1 ;
	} else
		cout<<0 ;
	return 0 ;
}

