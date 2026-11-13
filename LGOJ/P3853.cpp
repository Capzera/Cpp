#include <bits/stdc++.h>
using namespace std;
long long n, L, k, a[100001], ans;
bool check(int x) {
	if (!x) return 0;
	int cnt = 0;
	for (int i = 2; i <= n; i++) {
		if (a[i] - a[i - 1] <= x) continue;
		cnt += (a[i] - a[i - 1] + x - 1) / x - 1;
		if (cnt > k) return 0;
	}
	return 1;
}
int main() {
	cin >> L >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	int l = 0, r = L;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			r = ans = mid;
		} else l = mid + 1;
	}
	cout << ans;
	return 0;
}
