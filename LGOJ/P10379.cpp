#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, m, a[501][501], vis[501][501];
vector<vector<int> > block;
set<vector<int> > mp;
void dfs(int i, int j) {
	for (int d = 0; d < 4; d++) {
		int x = i + dx[d], y = j + dy[d];
		if (x < 0 || x >= n || y < 0 || y >= m || vis[x][y] || a[x][y] != a[i][j]) continue;
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
			dfs(i, j);
		}
	}
	for (auto& vec : block) {
		sort(vec.begin(), vec.end());
		int st = *vec.begin();
		for (auto& x : vec) x -= st;
		mp.insert(vec);
	}
	cout << mp.size();
	return 0;
}

