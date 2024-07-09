#include <bits/stdc++.h>
using namespace std;
int main () {
	int k, m = 0 ;
	cin >> k ;
	int i = 1 ;
	while (k != 0) {
		if (k < i) {
			m = m + k * i ;
			k = 0 ;
		} else {
			k = k - i ;
			m = m + i * i;
		}
		i = i + 1 ;
	}
	cout << m ;
	return 0;
}
