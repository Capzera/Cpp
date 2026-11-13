#include <bits/stdc++.h>
using namespace std;
long long n, m, k, x, ans;
int main() {
	cin >> n >> m >> k;
	while (n--) {
		long long mx = INT_MIN;
		for (int i = 1; i <= m; i++) {
			cin >> x;
			mx = max(mx, x - k * i);
		}
		ans += mx;
	}
	cout << ans;
	return 0;
}
