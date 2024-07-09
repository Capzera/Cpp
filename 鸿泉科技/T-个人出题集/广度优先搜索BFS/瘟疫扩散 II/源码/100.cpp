#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, k, x, y;
	int dx[] = {1, -1, 0, 0};
	int dy[] = {0, 0, 1, -1};
	cin >> n >> m >> k >> x >> y;
	queue<pair<int, int> > q;
	set<pair<int, int> > s;
	vector<vector<int> > a(n, vector<int>(m, -1));
	q.push({x, y});
	a[x][y] = 0;
	while (k--) {
		cin >> x >> y;
		s.insert({x, y});
	}
	while (q.size()) {
		int x = q.front().first, y = q.front().second; q.pop();
		for (int d = 0; d < 4; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 0 || mx == n || my < 0 || my == m || a[mx][my] >= 0 || s.count({mx, my})) continue;
			q.push({mx, my});
			a[mx][my] = a[x][y] + 1;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}
