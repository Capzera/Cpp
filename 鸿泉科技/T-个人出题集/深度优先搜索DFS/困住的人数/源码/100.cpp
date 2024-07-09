#include <bits/stdc++.h>
using namespace std;
vector<vector<int> > grid;
vector<vector<int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
int m, n;
vector<vector<bool>> visited;
void dfs(const vector<vector<int>> & grid, int row, int col) {
	if (row < 0 || row >= m || col < 0 || col >= n || grid[row][col] == 0 || visited[row][col]) {
		return;
	}
	visited[row][col] = true;
	for (auto & dir : dirs) {
		dfs(grid, row + dir[0], col + dir[1]);
	}
}
int main() {
	cin >> m;
	cin >> n;
	grid.clear();
	visited.clear();
	grid = vector<vector<int> > (m, vector<int>(n));
	visited = vector<vector<bool> > (m, vector<bool>(n));
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cin >> grid[i][j];
		}
	}	
	for (int i = 0; i < m; i++) {
		dfs(grid, i, 0);
		dfs(grid, i, n - 1);
	}
	for (int j = 1; j < n - 1; j++) {
		dfs(grid, 0, j);
		dfs(grid, m - 1, j);
	}
	int enclaves = 0;
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			if (grid[i][j] == 1 && !visited[i][j]) {
				enclaves++;
			}
		}
	}
	cout << enclaves;
	return 0;
}
