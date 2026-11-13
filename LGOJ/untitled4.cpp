#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, m, a[501][501], vis[501][501];
vector<vector<int> > block;
void dfs(int i, int j) {
	for (int d = 0; d < 4; d++) {
		int x = i + dx[d], y = j + dy[d];
		if (x < 0 || x >= n || y < 0 || y >= m || vis[x][y]) continue;
		vis[x][y] = 1;
		block.back().push_back(x * m + y);
		dfs(x, y);
	}
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> a[i][j];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (vis[i][j]) continue;
			vis[i][j] = 1;
			block.push_back({i * m + j});
		}
	}
	for (auto& vec : block) {
		for (auto& x : vec) cout << x << " ";
		puts("");
	}
	return 0;
}

