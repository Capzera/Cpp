#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x;
	cin >> n;
	int st = INT_MIN, nd = INT_MIN;
	for (int i = 0; i < n; i++) {
		cin >> x;
		if (x > st) {
			nd = st;
			st = x;
		} else if (x != st && x > nd) {
			nd = x;
		}
	}
	if (nd == INT_MIN) cout << "No" << endl;
	else cout << nd << endl;
	return 0;
}
