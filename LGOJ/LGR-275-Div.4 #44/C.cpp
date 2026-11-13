#include <bits/stdc++.h>
using namespace std;
int n, x, y, k, mx, ans;
int main() {
	cin >> n >> x >> y;
	for (int i = 1; i <= n; i++) {
		cin >> k;
		if (x + k == y || y + k == x || x + y == k || x * k == y || y * k == x || x * y == k) {
			ans++;
			mx = max(mx, k);
		}
	}
	cout << ans << " " << mx;
	return 0;
}
