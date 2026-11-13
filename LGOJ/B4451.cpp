#include <bits/stdc++.h>
using namespace std;
long long n, m, h, ans, a[1001][1001];
int main() {
	cin >> n >> m >> h;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	for (int i = 1; i <= n - 2; i++) {
		for (int j = 1; j <= m - 2; j++) {
			long long mx = INT_MIN, mn = INT_MAX, sum = 0;
			for (int x = 1; x <= 3; x++) {
				for (int y = 1; y <= 3; y++) {
					long long num = a[i + x - 1][j + y - 1];
					sum += num;
					mx = max(mx, num);
					mn = min(mn, num);
				}
			}
			if (mx - mn <= h) ans = max(ans, sum);
		}
	}
	cout << ans;
	return 0;
}
