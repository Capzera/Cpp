#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, sx, sy, tx, ty, a[N][N];
char ch;
struct point{int x, y;};
queue<point> q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> ch;
			a[i][j] = ch - '0';
		}
	}
	cin >> sx >> sy >> tx >> ty;
	q.push({sx, sy});
	while (q.size()) {
		int x = q.front().x, y = q.front().y; q.pop();
		if (x == tx && y == ty) {
			cout << a[x][y];
			return 0;
		}
		for (int d = 0; d < 4; d++) {
			int nx = x + dx[d], ny = y + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > n || (nx == sx && ny == sy) || a[nx][ny]) continue;
			a[nx][ny] = a[x][y] + 1;
			q.push({nx, ny});
		}
	}
	return 0;
}
