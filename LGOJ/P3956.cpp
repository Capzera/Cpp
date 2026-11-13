#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, 0, -1, 0, 2, 0, -2, 0, -1, -1, 1, 1};
const int dy[] = {0, 1, 0, -1, 0, 2, 0, -2, -1, 1, -1, 1};
const int dw[] = {0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2};
int m, n, x, y, w, nc;
int c[101][101], dis[101][101];
queue<vector<int> > q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	while (m--) {
		cin >> x >> y >> w;
		c[x][y] = w + 1;
	}
	memset(dis, 0x3f, sizeof(dis));
	dis[1][1] = 0;
	q.push({1, 1, c[1][1], 0});
	while (q.size()) {
		x = q.front()[0], y = q.front()[1], w = q.front()[3], nc= q.front()[2]; q.pop();
		if (dis[x][y] < w) continue;
		for (int d = 0; d < 12; d++) {
			int nx = x + dx[d], ny = y + dy[d], nw = w + dw[d];
			if (nx < 1 || nx > n || ny < 1 || ny > n || !c[nx][ny]) continue;
			if (nc != c[nx][ny]) nw++;
			if (dis[nx][ny] > nw) {
				dis[nx][ny] = nw;
				q.push({nx, ny, c[nx][ny], nw});
			}
		}
	}
	int ans = dis[n][n];
	if (!c[n][n]) {
		ans = min(dis[n][n - 1], dis[n - 1][n]) + 2;
	}
	if (ans >= 0x3f3f3f) ans = -1;
	cout << ans;
	return 0;
}
