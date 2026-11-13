#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
struct node {
	int x, y, val;
};
int n, m, sx, sy, vis[301][301];
vector<pair<int, int> > alpha[26];
char a[301][301];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == '@') sx = i, sy = j;
			if (isalpha(a[i][j])) alpha[a[i][j] - 'A'].push_back({i, j});
		}
	}
	queue<node> q;
	q.push({sx, sy, 0});
	vis[sx][sy] = 0;
	while (q.size()) {
		int x = q.front().x, y = q.front().y, val = q.front().val; q.pop();
		if (a[x][y] == '=') {
			cout << val;
			return 0;
		}
		if (isalpha(a[x][y])) {
			int k = a[x][y] - 'A';
			int another = (make_pair(x, y) == alpha[k][0]);
			x = alpha[k][another].first, y = alpha[k][another].second;
		}
		for (int d = 0; d < 4; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 1 || mx > n || my < 1 || my > m || vis[mx][my] || a[mx][my] == '#') continue;
			vis[mx][my] = 1;
			q.push({mx, my, val + 1});
		}
	}	
	return 0;
}
