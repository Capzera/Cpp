#include <bits/stdc++.h>
using namespace std;
const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
vector<vector<int> > grid, vis;
int n, peak, valley;
bool f1, f2;
void dfs(int x, int y, int v) {
	vis[x][y] = 1;
	for (int d = 0; d < 8; d++) {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 0 || mx == n || my < 0 || my == n) continue;
		if (!vis[mx][my] && grid[mx][my] == v) dfs(mx, my, v);
		if (grid[mx][my] > v) f1 = 1;
		if (grid[mx][my] < v) f2 = 1;
	}
}
int main() {
	cin >> n;
	grid = vis = vector<vector<int> > (n, vector<int>(n));
	for (auto& i : grid)
		for (auto& j : i) cin >> j;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (vis[i][j]) continue;
			f1 = f2 = 0;
			dfs(i, j, grid[i][j]);
			if (f1 && !f2) valley++;
			if (!f1 && f2) peak++;
		}
	}
	if (peak == valley && !peak) peak = valley = 1;
	cout << peak << " " << valley;
	return 0;
}
