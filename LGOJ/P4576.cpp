#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1, 0, 2, 0, -2}, dy[] = {1, 0, -1, 0, 2, 0, -2, 0};
int r1, c1, r2, c2, n;
int f[2][60][21][21][21][21];
int dfs(int x, int y, int x1, int y1, int x2, int y2) {
	int ans = !x ? 0 : 0x3f3f3f3f;
	if (y > 3 * n) return 0x3f3f3f3f;
	if (f[x][y][x1][y1][x2][y2]) return f[x][y][x1][y1][x2][y2];
	if (x1 == x2 && y1 == y2) return x ? 0x3f3f3f3f : 0;
	if (!x) {
		for (int d = 0; d < 4; d++) {
			int nx = x1 + dx[d], ny = y1 + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > n) continue;
			ans = max(ans, dfs(x ^ 1, y + 1, nx, ny, x2, y2));
		}
	} else {
		for (int d = 0; d < 8; d++) {
			int nx = x2 + dx[d], ny = y2 + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > n) continue;
			ans = min(ans, dfs(x ^ 1, y + 1, x1, y1, nx, ny));
		}
	}
	return f[x][y][x1][y1][x2][y2] = ++ans;
} 

int main() {
	cin >> n >> r1 >> c1 >> r2 >> c2;
	if (abs(r1 - r2) + abs(c1 - c2) <= 1) return puts("WHITE 1"), 0;
	cout << "BLACK " << dfs(0, 0, r1, c1, r2, c2);
	return 0;
}
