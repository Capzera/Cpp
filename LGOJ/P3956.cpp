#include <bits/stdc++.h>
using namespace std;
const int maxn = 105;
const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
int a[maxn][maxn], ans[maxn][maxn];
int m, n, x, y, z;
void print() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (ans[i][j] == 0x3f3f3f3f) cout << 0 << " ";
			else cout << ans[i][j] << " ";	
		}
		cout << endl;
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++) a[i][j] = -1, ans[i][j] = 0x3f3f3f3f;
	for (int i = 0; i < m; i++) {
		cin >> x >> y >> z;
		a[x][y] = z;
	}
	ans[1][1] = 0;
	queue<vector<int> > q;
	q.push({1, 1, a[1][1], 0});
	while (q.size()) {
		x = q.front()[0], y = q.front()[1];
		if (x == n && y == n) {
			print();
			cout << ans[n][n];
			return 0;
		}
		int color = q.front()[2], magic = q.front()[3];
		q.pop();
		for (int d = 0; d < 4; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 1 || mx > n || my < 1 || my > n) continue;
			if (a[mx][my] == -1 && magic) continue;
			if (a[mx][my] == -1) { //色-白
				if (ans[mx][my] <= ans[x][y] + 2) continue;
				ans[mx][my] = ans[x][y] + 2;

				q.push({mx, my, color, 1});
			} else if (a[mx][my] == color) { //色-同
				if (ans[mx][my] <= ans[x][y]) continue;
				ans[mx][my] = ans[x][y];
				//cout << a[x][y] << " " << color << " " << a[mx][my] << endl;
				q.push({mx, my, color, 0});
			} else { //色-异
				if (ans[mx][my] <= ans[x][y] + 1) continue;
				ans[mx][my] = ans[x][y] + 1;
				//cout << a[x][y] << " " << color << " " << a[mx][my] << endl;
				q.push({mx, my, a[mx][my], 0});
			}
		}
	}
	cout << -1;
	return 0;
}
/*
21 81
1 1 0
1 3 0
1 5 0
1 6 0
2 1 1
2 5 1
3 7 1
3 3 0
3 2 0
4 3 0
5 5 0
5 1 0
5 7 1
5 4 1
6 3 0
7 4 1
7 6 1
7 2 1
7 12 0
7 7 0
7 3 1
8 11 0
8 8 1
8 12 1
9 6 0
9 5 1
9 7 1
9 4 0
9 13 0
9 11 1
10 13 1
10 4 1
10 10 0
10 8 0
10 11 1
10 5 0
10 9 1
11 5 0
11 8 1
11 12 1
11 16 1
11 14 1
12 16 1
12 11 0
13 15 1
13 9 1
13 10 0
13 17 1
13 13 0
13 11 0
13 8 0
14 14 0
15 10 0
15 14 1
15 15 0
15 9 0
15 17 1
15 18 0
15 12 0
15 20 0
16 15 0
16 12 0
16 17 1
16 21 1
16 11 0
17 11 1
17 13 0
17 17 1
18 20 0
18 21 1
18 17 0
18 16 1
18 14 0
19 15 1
19 18 0
19 19 0
20 19 1
21 20 0
21 18 0
21 15 1
21 16 1

*/
