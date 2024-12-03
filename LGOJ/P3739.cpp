#include <bits/stdc++.h>
using namespace std;
long long n, m, k, s, x, y, r, c, mn = LLONG_MAX;
long long get(int x1, int y1, int x2, int y2) {
	if (x1 > x2) swap(x1, x2), swap(y1, y2);
	int t = x2 - x1 << 1;
	if (y2 >= y1 + t) return y2 - y1;
	if (y2 <= y1) return y1 - y2 + t;
	return t + (y2 & 1) - (y1 & 1);
}
int main() {
	cin >> n >> m >> k >> s >> x >> y;
	while (m--) {
		cin >> r >> c;
		mn = min(mn, get(x, y, r, c));
	}
	cout << max(s - mn * k, 0ll) - 1;
	return 0;
}
