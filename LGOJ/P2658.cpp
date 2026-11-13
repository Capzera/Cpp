#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int n, m, x, ans, cnt, sx, sy, vis[501][501], sign[501][501];
bool check(int mid, vector<vector<int> > a) {
	int ans = 1;
	queue<pair<int, int> > q;
	memset(vis, 0, sizeof(vis));
	q.push({sx, sy});
	vis[sx][sy] = 1;
	while (q.size()) {
		auto [x, y] = q.front(); q.pop();
		if (ans == cnt) return 1;
		for (int d = 0; d < 4; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 0 || mx == n || my < 0 || my == m || vis[mx][my]) continue;
			int diff = abs(a[mx][my] - a[x][y]);
			if (diff > mid) continue;
			if (sign[mx][my]) ans++;
			q.push({mx, my});
			vis[mx][my] = 1;
		}
	}
	return 0;
}
int main() {
	cin >> n >> m;
	vector<vector<int> > a(n, vector<int>(m));
	int l = 0, r = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> a[i][j];
			r = max(r, a[i][j]);
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> x;
			if (!x) continue;
			sign[i][j] = 1;
			if (++cnt == 1) sx = i, sy = j;
		}
	}
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid, a)) {
			ans = r = mid;
		} else l = mid + 1;
	}
	cout << ans;
	return 0;
}
