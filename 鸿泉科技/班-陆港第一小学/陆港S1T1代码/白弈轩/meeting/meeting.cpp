#include <bits/stdc++.h>
using namespace std;
int main () {
	int t,m,s,v,d,x,y,a;
	cin>>t>>m>>s>>v;
	d=100/99+1;
	x=(22*60-(m+d))/60;
	y=60-(m+d);
	cout<<x<<" "<<y;
	return 0;
}
