#include <bits/stdc++.h>
using namespace std;
int n, k, q, x, y, p[1010];
long long solve(int l, int r) {
	if (l == r) return 1;
	int za = upper_bound(p, p + k + 1, x) - p;
	int zb = upper_bound(p, p + k + 1, y) - p;
	if (za == zb) return 2;
	int m = zb - za - 1;
	if (x == p[za - 1]) m++;
	if (y == p[zb] - 1) m++;
	return m + 2;
}
int main() {
	cin >> n >> k;
	for (int i = 0; i <= k; i++) cin >> p[i];
	cin >> q;
	while (q--) {
		cin >> x >> y;
		if (x > y) swap(x, y);
		cout << solve(x, y) << endl;
	}
	return 0;
}
