#include <bits/stdc++.h>
using namespace std;
int wood[51], bucket[51], sum[1001], a[1001], m, n, tot;
bool check(int last, int x) { // 第i个桶，还剩余的工作有x个是否可行
	if (x <= 0) return 1;
	for (int i = last; i <= m; i++) {
		if (bucket[i] + a[x] > wood[i]) continue;
		bucket[i] += a[x];
		if (check(a[x] == a[x - 1] ? i : 1, x - 1)) return 1;
		bucket[i] -= a[x];
	}
	return 0;
}
int main() {
	cin >> m;
	for (int i = 1; i <= m; i++) {
		cin >> wood[i];
		tot += wood[i];
	}
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(wood + 1, wood + m + 1);
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		sum[i] = sum[i - 1] + a[i];
	}
	while (tot < sum[n]) n--;
	int l = 0, r = n + 1, ans = 0;
	while (l < r) {
		int mid = l + (r - l >> 1);
		memset(bucket, 0, sizeof(bucket));
		if (check(1, mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans;
	return 0;
}
/*
25 30 40 50
1  2  3  4  5  6  7  8  9  10
15 16 17 18 19 20 21 24 25 30
*/

