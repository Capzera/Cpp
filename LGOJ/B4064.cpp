#include <bits/stdc++.h>
using namespace std;
int t, a, x, ans;
int main() {
	cin >> t;
	while (t--) {
		cin >> a;
		bool f = 1;
		for (int i = 1; i <= 100; i++) {
			if (pow(i, 4) == a) {
				cout << i << endl;
				f = 0;
				break;
			}
		}
		if (f) cout << -1 << endl;
	}
	
	return 0;
}
