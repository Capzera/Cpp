#include <bits/stdc++.h>
using namespace std;
struct points {
	int x, y, flag;
} a[1001];
long long n, k, t, x, y, j, ans;
int get(int x1, int y1, int x2, int y2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
int main() {
	cin >> n >> k >> t;
	for (int i = 1; i <= n; i++) cin >> a[i].x >> a[i].y;
	while (k--) {
		cin >> x >> y;
		for (int i = 1; i <= n; i++) {
			if (a[i].x == x && a[i].y == y) {
				a[i].flag = true;
				break;
			}
		}
	}
	while (t--) {
		cin >> x >> y;
		long long mx = 0;
		for (int i = 1; i <= n; i++) {
			if (get(x, y, a[i].x, a[i].y) > mx) {
				j = i;
				mx = get(x, y, a[i].x, a[i].y);
			}
		}
		ans += a[j].flag;
	}
	cout << ans;
}
