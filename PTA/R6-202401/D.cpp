#include <bits/stdc++.h>
using namespace std;
long long n, m, a[100010];
bool check(int x) {
	int ans = 1;
	for (int i = 2, lst = 1; i <= n; i++) {
		if (a[i] - a[lst] < x) continue;
		lst = i;
		if (++ans >= m) return 1;
	}
	return ans >= m;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	int l = 0, r = 2e9, ans = 0;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans;
	return 0;
}
