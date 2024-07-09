#include <bits/stdc++.h>
using namespace std;
vector<string> board;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};
int m, n;
void dfs(int x, int y) {
	if (x < 0 || x >= m || y < 0 || y >= n || board[x][y] != 'O') {
		return;
	}
	board[x][y] = 'A';
	for (int d = 0; d < 4; d++) {
		int nx = x + dx[d];
		int ny = y + dy[d];
		dfs(nx, ny);
	}
}
int main() {
	cin >> m;
	cin >> n;
	board.resize(m);
	for (int i = 0; i < m; i++) {
		cin >> board[i];
	}
	for (int i = 0; i < m; i++) {
		dfs(i, 0);
		dfs(i, n - 1);
	}
	for (int i = 1; i < n - 1; i++) {
		dfs(0, i);
		dfs(m - 1, i);
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] == 'A') {
				board[i][j] = 'O';
			} else if (board[i][j] == 'O') {
				board[i][j] = 'X';
			}
		}
	}
	for (int i = 0; i < m; i++) {
		cout << board[i] << endl;
	}
	return 0;
}
