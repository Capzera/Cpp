#include <bits/stdc++.h>
using namespace std;
const int N = 10;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
struct node {
	int x, y, hp;
};
int n, m, sx, sy, fx, fy, ans[N][N][7], a[N][N];
queue<node> q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == 2) sx = i, sy = j;
			if (a[i][j] == 3) fx = i, fy = j;
		}
	}
	memset(ans, -1, sizeof ans);
	
	q.push({sx, sy, 6});
	ans[sx][sy][6] = 0;
	while (q.size()) {
		int x = q.front().x, y = q.front().y, hp = q.front().hp; q.pop();
		if (x == fx && y == fy) {
			cout << ans[x][y][hp];
			return 0;
		}
		for (int d = 0; d < 4; d++) {
			int nx = x + dx[d], ny = y + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > m || !a[nx][ny] || hp == 1 || ans[nx][ny][hp - 1] != -1) continue;
			int nhp = a[nx][ny] == 4 ? 6 : hp - 1;
			ans[nx][ny][nhp] = ans[x][y][hp] + 1;
			q.push({nx, ny, nhp});
		}
	}
	puts("-1");
	return 0;
}

