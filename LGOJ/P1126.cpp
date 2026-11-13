#include <bits/stdc++.h>
using namespace std;
int main() {
	int dx[] = {1, 0, -1, 0};
	int dy[] = {0, 1, 0, -1};
	int m, n, sx, sy, fx, fy, di;
	char ch;
	cin >> m >> n;
	vector<vector<int> > grid(m, vector<int> (n));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) cin >> grid[i][j];
	vector<vector<vector<int> > > a(m, vector<vector<int> > (n, vector<int>(4, -1)));
	cin >> sx >> sy >> fx >> fy >> ch;
	if (ch == 'S') di = 0;
	if (ch == 'E') di = 1;
	if (ch == 'N') di = 2;
	if (ch == 'W') di = 3;
	queue<vector<int> > q;
	q.push({sx, sy, di});
	a[sx][sy][di] = 0;
	while (q.size()) {
		int nx = q.front()[0], ny = q.front()[1], nd = q.front()[2]; q.pop();
		if (nx == fx && ny == fy) {
			cout << a[fx][fy][nd] << endl;
			return 0;
		}
		for (int i = 1; i <= 3; i++) {
			int mx = nx + dx[nd] * i, my = ny + dy[nd] * i;
			if (mx < 1 || mx >= m || my < 1 || my >= n || a[mx][my][nd] != -1) continue;
			if (grid[mx - 1][my - 1] || grid[mx - 1][my] || grid[mx][my - 1] || grid[mx][my]) break;
			a[mx][my][nd] = a[nx][ny][nd] + 1;
			q.push({mx, my, nd});
		}
		int md = (nd + 1) % 4;
		if (a[nx][ny][md] == -1) {
			a[nx][ny][md] = a[nx][ny][nd] + 1;
			q.push({nx, ny, md});
		}
		md = (nd + 3) % 4;
		if (a[nx][ny][md] == -1) {
			a[nx][ny][md] = a[nx][ny][nd] + 1;
			q.push({nx, ny, md});
		}
	}
	cout << -1 << endl;
}
