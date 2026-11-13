#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
int n, m, a[31][31];
void dfs(int x, int y) {
	if (x < 1 || x > n || y < 1 || y > n || a[x][y]) return;
	a[x][y] = 3;
	for (int d = 0; d < 4; d++) {
		int mx = x + dx[d], my = y + dy[d];
		dfs(mx, my);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n; i++) {
		dfs(i, 1);
		dfs(i, n);
	}
	for (int j = 1; j <= n; j++) {
		dfs(1, j);
		dfs(n, j);
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (a[i][j] == 3) {
				a[i][j] = 0;
			} else if (a[i][j] == 0) {
				a[i][j] = 2;
			}
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
