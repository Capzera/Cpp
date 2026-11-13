#include <bits/stdc++.h>
using namespace std;
int n, x, m, ans = INT_MAX;
int main() {
	cin >> n >> m;
	while (n--) {
		int mx = INT_MIN;
		for (int i = 1; i <= m; i++) {
			cin >> x;
			mx = max(mx, x);
		}
		ans = min(ans, mx);
	}
	cout << ans;
	return 0;
}
