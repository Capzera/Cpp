#include <bits/stdc++.h>
using namespace std;
int main () {
	int n ;
	cin >> n ;
	if (n == 1)
		cout << "2" ;
	if (n == 2)
		cout << "3" ;
	if ( n > 2 ) {
		int m = 3 ;
		int i = 2 ;
		for (int j = n - 2 ; j > 0 ; j-- ) {
			m = m + i ;
			i++ ;
		}
		cout << m ;
	}

	return 0;
}
