#include <bits/stdc++.h>
using namespace std;
const int N = 305, dx[] = {0, 1, 0, -1, 0}, dy[] = {0, 0, 1, 0, -1};
struct node {
	int x, y, val;
};
int n, x, y, t, vis[N][N];
int main() {
	vector<vector<int> > a(N, vector<int>(N, INT_MAX));
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x >> y >> t;
		for (int d = 0; d < 5; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 0 || my < 0) continue;
			a[mx][my] = min(a[mx][my], t);
		}
	}
	queue<node> q;
	q.push({0, 0, 0});
	vis[0][0] = 1;
	while (q.size()) {
		int x = q.front().x, y = q.front().y, val = q.front().val; q.pop();
		if (a[x][y] == INT_MAX) return cout << val, 0;
		for (int d = 1; d <= 4; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 0 || my < 0 || vis[mx][my] || val + 1 >= a[mx][my]) continue;
			vis[mx][my] = 1;
			q.push({mx, my, val + 1});
		}
	}
	puts("-1");
	return 0;
}
