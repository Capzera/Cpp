#include <bits/stdc++.h>
using namespace std;
int n, cnt, ans = 1;
int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
vector<int> area = {0, 0};
vector<vector<int> > grid;
void dfs(int x, int y) {
	if (x >= n || x < 0 || y >= n || y < 0 || grid[x][y] != 1) return;
	cnt++;
	grid[x][y] = area.size();
	for (int d = 0; d < 4; d++){
		int mx = x + dx[d], my = y + dy[d];
		dfs(mx, my);
	}
}
int main() {
	cin >> n;
	grid = vector<vector<int> > (n, vector<int>(n));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> grid[i][j];
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++){
			if (grid[i][j] != 1) continue;
			cnt = 0;
			dfs(i, j);
			area.push_back(cnt);
			ans = max(ans, cnt);
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (grid[i][j]) continue;
			int sum = 1;
			set<int> s;
			for (int d = 0; d < 4; d++){
				int x = i + dx[d], y = j + dy[d];
				if (x >= n || x < 0 || y >= n || y < 0 || !grid[x][y] || s.count(grid[x][y])) continue;
				s.insert(grid[x][y]);
				sum += area[grid[x][y]];
			}
			ans = max(ans, sum);
		}
	}
	cout << ans;
	return 0;
}
