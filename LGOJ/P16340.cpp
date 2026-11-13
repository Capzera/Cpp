#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int t, n, a[N], diff[N];
int get(int x) {
	if (!x) return -1;
	return 31 - __builtin_clz(x);
}
int solve() {
	scanf("%d", &n);
	int ans = 0;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	sort(a + 1, a + n + 1);
	diff[0] = diff[n] = INT_MAX;
	for (int i = 1; i < n; i++) {
		diff[i] = get(a[i] ^ a[i + 1]);
	}
	for (int i = 1; i < n; i++) {
		ans += (diff[i] < diff[i - 1]) && (diff[i] < diff[i + 1]);
	}
	return ans;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
