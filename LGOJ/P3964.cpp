#include <bits/stdc++.h>
using namespace std;
const int N = 100010;
long long n, ax[N], ay[N], x[N], y[N], sx[N], sy[N], ans = LLONG_MAX;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x[i] >> y[i];
		x[i] = ax[i] = x[i] + y[i];
		y[i] = ay[i] = x[i] - 2 * y[i];
	}
	sort(x + 1, x + n + 1), sort(y + 1, y + n + 1);
	for (int i = 1; i <= n; i++) {
		sx[i] = sx[i - 1] + x[i];
		sy[i] = sy[i - 1] + y[i];
	}
	for (int i = 1; i <= n; i++) {
		int px = lower_bound(x + 1, x + n + 1, ax[i]) - x;
		int py = lower_bound(y + 1, y + n + 1, ay[i]) - y;
		long long sumx = (2 * px - n) * ax[i] + sx[n] - 2 * sx[px];
		long long sumy = (2 * py - n) * ay[i] + sy[n] - 2 * sy[py];
		ans = min(ans, sumx + sumy);
	}
	cout << ans / 2;
	return 0;
}
