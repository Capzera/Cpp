#include <bits/stdc++.h>
using namespace std;
int a[4], b[4], c, mx, ans = INT_MAX, loc = -1;
int main() {
	for (int i = 1; i <= 3; i++) {
		cin >> a[i] >> b[i];
		mx = max(mx, b[i]);
	}
	cin >> c;
	for (int i = 1; i <= 3; i++) {
		if (a[i] - c * (mx - b[i]) < ans) {
			ans = a[i] - c * (mx - b[i]);
			loc = i;
		}
	}
	cout << loc << " " << ans;
	return 0;
}
