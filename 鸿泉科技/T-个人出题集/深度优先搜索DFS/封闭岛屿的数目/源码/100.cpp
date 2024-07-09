#include <bits/stdc++.h>
using namespace std;
vector<vector<int> > grid;
vector<int> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
int ans = 0, m, n, flag;
void dfs(int x, int y, vector<vector<int>>& grid) {
	if (grid[x][y]) return;
	grid[x][y] = 1;
	for (int d = 0; d < 4; d++) {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 0 || mx == m || my < 0 || my == n) {
			flag = 0;
			continue;
		}
		dfs(mx, my, grid);
	}
}
int main() {
	cin >> m;
	cin >> n;
	grid = vector<vector<int> > (m, vector<int>(n));
	ans = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cin >> grid[i][j];
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0 ; j < n; j++) {
			if (grid[i][j]) continue;
			flag = 1;
			dfs(i, j, grid);
			ans += flag;
		}
	}
	cout << ans << endl;
	return 0;
}
