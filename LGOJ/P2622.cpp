#include <bits/stdc++.h>
using namespace std;
long long n, m, dp[1 << 10], a[101][10], vis[1 << 10];
int bfs() {
	queue<pair<int, int> > q;
	q.push({(1 << n) - 1, 0});
	vis[(1 << n) - 1] = 1;
	while (q.size()) {
		int u = q.front().first, step = q.front().second; q.pop();
		if (!u) return step;
		for (int i = 0; i < m; i++) {
			int mask = u;
			for (int j = 0; j < n; j++) {
				if (a[i][j] == 1) mask &= ~(1 << j);
				if (a[i][j] == -1) mask |= 1 << j;
			}
			if (!vis[mask]) {
				vis[mask] = 1;
				q.push({mask, step + 1});
			}
		}
	}
	return -1;
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) 
			cin >> a[i][j];
	cout << bfs();
	return 0;
}
/*
dp[i] 表示状态为i的时候，最少需要几步操作
dp[i] = min ()

*/
