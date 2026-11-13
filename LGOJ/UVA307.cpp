#include <bits/stdc++.h>
using namespace std;
vector<int> vis;
int t, n, sum, mx, a[70];
int dfs(int cur, int st, int cnt, int d) {
	if (cnt == sum / d) return 1;
	if (cur == d) return dfs(0, 0, cnt + 1, d);
	for (int i = st, lst = -1; i < n; i++) {
		if (vis[i] || a[i] + cur > d || a[i] == a[lst]) continue;
		vis[i] = 1;
		if (dfs(cur + a[i], i + 1, cnt, d)) return 1;
		vis[i] = 0;
		lst = i;
		if (!cur) return 0;
		if (cur + a[i] == d) return 0;
	}
	return 0;
}
int main() {
	while (cin >> n, n) {
		sum = 0, mx = 0;
		for (int i = 0; i < n; i++) {
			scanf("%d", &a[i]);
			sum += a[i];
			mx = max(mx, a[i]);
		}
		sort(a, a + n, greater<int>());
		for (int d = mx; d <= sum; d++) {
			if (sum % d) continue;
			vis = vector<int>(n, 0);
			if (!dfs(0, 0, 0, d)) continue;
			printf("%d\n", d);
			break;
		}
	}
}
