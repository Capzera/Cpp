#include <bits/stdc++.h>
using namespace std;
int n, d, m, a[1000001];
bool check(int x) { // 有x台机器，是否能够完成任务
	int cnt = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= x; j++) {
			if (a[cnt] > i) break; // 1 1 2 2 2 3 3 4 4 5 6 6
			if (a[cnt] + d < i) return 0;
			if (++cnt == m) return 1;
		}
	}
	return 0;
}
int main() {
	cin >> n >> d >> m;
	for (int i = 1; i <= m; i++) cin >> a[i];
	sort(a + 1, a + m + 1);
	int l = 1, r = m + 1, ans = 0;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) ans = r = mid;
		else l = mid + 1;
	}
	cout << ans;
	return 0;
}
