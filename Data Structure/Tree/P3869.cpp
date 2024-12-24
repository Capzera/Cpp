#include <bits/stdc++.h>
using namespace std;
struct node {
	long long x, y, dis, s;
};
const int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
long long n, m, sx, sy, ex, ey, p, r1[31], c1[31], r2[31], c2[31], ans[901];
queue<node> q;
char a[31][31];
map<int, vector<int> > mp;
bool vis[31][31][1 << 11];

int bfs() {
	while (q.size()) {
		node u = q.front(); q.pop();
		if (u.x == ex && u.y == ey) return u.dis;
		for (int d = 0; d < 4; d++) {
			int mx = u.x + dx[d], my = u.y + dy[d];
			if (mx < 1 || mx > n || my < 1 || my > m) continue;
			int f = a[mx][my] != '#', s = u.s;
			for (int j = 0; j < p; j++) {
				if (mx == r1[j] && my == c1[j]) s ^= 1 << j;
				if (mx == r2[j] && my == c2[j] && u.s >> j & 1) f ^= 1; 
			}
			if (!f || vis[mx][my][s]) continue;
			q.push({mx, my, u.dis + 1, s});
			vis[mx][my][s] = 1;
		}
	}
	return -1;
}

int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == 'S') sx = i, sy = j;
			if (a[i][j] == 'T') ex = i, ey = j;
		}
	}
	cin >> p;
	for (int i = 0; i < p; i++) cin >> r1[i] >> c1[i] >> r2[i] >> c2[i];
	q.push({sx, sy, 0, 0}); vis[sx][sy][0] = 1;
	cout << bfs();
	return 0;
}
