#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x, ans = -1, mx = INT_MIN;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		if (x >= mx) {
			mx = x;
			ans = i;
		}
	}
	cout << ans;
	return 0;
}
