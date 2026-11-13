#include <bits/stdc++.h>
using namespace std;
const int N = 505;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, m, k, p, x, y, a[N][N];
struct point {
	int x, y;
};
queue<point> q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> k >> p;
	memset(a, -1, sizeof a);
	while (k--) {
		cin >> x >> y;
		a[x][y] = 0;
		q.push({x, y});
	}
	while (!q.empty()) {
		x = q.front().x, y = q.front().y; q.pop();
		for (int d = 0; d < 4; d++) {
			int nx = x + dx[d], ny = y + dy[d];
			if (nx < 1 || nx > n || y < 1 || ny > m || a[nx][ny] != -1) continue;
			a[nx][ny] = a[x][y] + 1;
			q.push({nx, ny});
		}
	}
	while (p--) {
		cin >> x >> y;
		cout << a[x][y] << '\n';
	}
	return 0;
}
