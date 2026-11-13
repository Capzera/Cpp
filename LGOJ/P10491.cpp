#include <bits/stdc++.h>
using namespace std;
const int N = 151;
const int dx[] = {-1, -2, -2, -1, 1, 2, 2, 1};
const int dy[] = {-2, -1, 1, 2, 2, 1, -1, -2};
int n, m, sx, sy, tx, ty, x, y, a[N][N];
char ch;
struct point {int x, y;};
queue<point> q;
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> ch;
			if (ch == '.') a[i][j] = -1;
			if (ch == '*') a[i][j] = -2;
			if (ch == 'K') sx = i, sy = j;
			if (ch == 'H') tx = i, ty = j, a[i][j] = -1;
		}
	}
	a[sx][sy] = 0;
	q.push({sx, sy});
	while (q.size()) {
		x = q.front().x, y = q.front().y; q.pop();
		if (x == tx && y == ty) {
			cout << a[x][y];
			return 0;
		}
		for (int d = 0; d < 8; d++) {
			int nx = x + dx[d], ny = y + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > m || a[nx][ny] != -1) continue;
			a[nx][ny] = a[x][y] + 1;
			q.push({nx, ny});
		}
	}
	return 0;
}
