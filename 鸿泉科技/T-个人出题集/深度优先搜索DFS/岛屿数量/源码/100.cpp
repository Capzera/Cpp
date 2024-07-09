#include <bits/stdc++.h>
using namespace std;
int m, n, ans = 0;
vector<int> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
vector<vector<int> > grid;
void dfs(int x, int y, vector<vector<int> >& grid) {
	grid[x][y] = 0;
	for (int d = 0; d < 4; d++) {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 0 || mx == m || my < 0 || my == n || grid[mx][my] == 0) continue;
		dfs(mx, my, grid);
	}
}
int main() {
	cin >> m;
	cin >> n;
	grid = vector<vector<int> > (m, vector<int>(n));
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cin >> grid[i][j];
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (grid[i][j] == 0) continue;
			dfs(i, j, grid);
			ans++;
		}
	}
	cout << ans << endl;
	return 0;
}
