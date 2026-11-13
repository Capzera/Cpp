#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, -1, 1, 0, 0}, dy[] = {0, 0, 0, -1, 1}; // 方向数组
long long n, m, q, x, y, ans, a[101][101], vis[101][101];
int check(int x, int y) {
	memset(vis, 0, sizeof(vis)); // 多测初值赋0
	int d = a[x][y], ans = 0;
	while (1) {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 1 || mx > n || my < 1 || my > m) return ans + 1; // 包含走出去本身1步
		if (vis[mx][my]) return -1; // 环
		ans++, x = mx, y = my, d = a[mx][my], vis[mx][my] = 1;
	}
}
int main() {
	cin >> n >> m >> q;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	while (q--) {
		cin >> x >> y;
		cout << check(x, y) << '\n';
	}
	return 0;
}
