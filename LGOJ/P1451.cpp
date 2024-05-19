#include <bits/stdc++.h>
using namespace std;
vector<string> g;
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};
int m, n, ans;
void bfs(int sx, int sy) {
	queue<pair<int, int> > q;
	q.push({sx, sy});
	while (q.size()) {
		int x = q.front().first, y = q.front().second; q.pop();
		g[x][y] = '0';
		for (int d = 0; d < 4; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 0 || mx == m || my < 0 || my == n || g[mx][my] == '0') continue;
			q.push({mx, my});
		}
	}
}
int main() {
	cin >> m >> n;
	g.resize(m);
	for (int i = 0; i < m; i++) {
		cin >> g[i];
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (g[i][j] == '0') continue;
			ans++;
			bfs(i, j);
		}
	}
	cout << ans << endl;
	return 0;
}
