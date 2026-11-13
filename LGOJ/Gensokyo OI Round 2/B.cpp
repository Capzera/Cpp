#include <bits/stdc++.h>
using namespace std;
int t, n, x;
int solve() {
	int ans = -1;
	cin >> n;
	int bit[31] = {0};
	for (int i = 1; i <= n; i++) {
		cin >> x;
		while (x) {
			bit[__builtin_ctz(x)]++;
			x &= x - 1;
		}
	}
	for (int k = 1; k < n; k++) {
		int f = 1, mask = 0;
		for (int i = 0; i < 30; i++) {
			if (bit[i] == n - k) {
				f = 0;
				break;
			}
			if (bit[i] > n - k) {
				mask += 1 << i;
			}
		}
		if (f) ans = max(ans, mask);
	}
	return ans;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
/*
a[i], a[j]
a[i]-> a[i] & a[j]
a[j]-> a[i] | a[j]
if j = i + 1 ??
0101010
1100101

0100000

1101111
操作的本质是0向左移，1向右移动

sumOr[1..k] == sumAnd[n - k + 1..n]


即给定k，求__builtin_popcount(x) 是否 >= n - k??

O(n * 30) = 1.5e8？
*/
