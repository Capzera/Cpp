#include <iostream>
using namespace std;
int main () {
	int a,b,c,d,n;
	cin >>n;
	if (n%2 == 1 && n%3 == 0) {
		a=1,b=1,c=1,d=0;
	} else if ((n%2 == 1 && n%3 != 0) || (n%2 != 1 && n%3 == 1)) {
		a=0,b=1,c=1,d=0;
	} else if (n%2 != 1 && n%3 != 0) {
		a=0,b=0,c=0,d=1;
	}
	cout <<a<<" "<<b<<" "<<c<<" "<<d;
	
	return 0;
}
