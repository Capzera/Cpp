#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x, mn = INT_MAX, mx = INT_MIN;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> x;
		mn = min(mn, x);
		mx = max(mx, x);
	}
	cout << mx - mn;
	return 0;
}
