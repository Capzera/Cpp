#include <bits/stdc++.h>
using namespace std;
long long loc = 0, n, c, x, mx = 0;
int main() {
	cin >> n >> c;
	long long ans = c * n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		mx = max(mx, x);
		if (mx + c * (n - i) < ans) {
			ans = mx + c * (n - i);
			loc = i;
		}
	}
	cout << loc << " " << ans;
	return 0;
}
