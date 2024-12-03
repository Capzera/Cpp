#include <bits/stdc++.h>
using namespace std;
int t, r1, c1, r2, c2;
int main() {
	cin >> t;
	while (t--) {
		cin >> r1 >> c1 >> r2 >> c2;
		if (__gcd(abs(r1 - r2), abs(c1 - c2)) == 1) cout << "no" << endl;
		else cout << "yes" << endl;
	}
	return 0;
}
