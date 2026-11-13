#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;
int n, m, k, w, cnt, ans = INT_MAX;
int a[N][N], vis[N][N], x[7], y[7];
int check(int p) {
	for (int mask = 0; mask < 1 << k; mask++) {
		cnt++;
		long long sum = 0;
		for (int i = 0; i < k; i++) {
			if (mask >> i & 1) {
				int l = max(x[i] - p, 1), r = min(x[i] + p, n), ny = y[i];
				for (int nx = l; nx <= r; nx++) {
					if (vis[nx][ny] == cnt) continue;
					vis[nx][ny] = cnt;
					sum += a[nx][ny];
				}
			} else {
				int l = max(y[i] - p, 1), r = min(y[i] + p, m), nx = x[i];
				for (int ny = l; ny <= r; ny++) {
					if (vis[nx][ny] == cnt) continue;
					vis[nx][ny] = cnt;
					sum += a[nx][ny];
				}
			}
		}
		if (sum >= w) return 1;
	}
	return 0;
}
int main() {
	scanf("%d%d%d%d", &n, &m, &k, &w);
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			scanf("%d", &a[i][j]);
	for (int i = 0; i < k; i++) scanf("%d%d", &x[i], &y[i]);
	int l = 0, r = max(n, m) + 1, ans = -1;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) ans = r = mid;
		else l = mid + 1;
	}
	if (ans == -1) return puts("Impossible"), 0;
	cout << ans;
	return 0;
}
