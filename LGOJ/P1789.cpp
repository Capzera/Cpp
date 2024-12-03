#include <bits/stdc++.h>
using namespace std;
long long n, m, k, x, y, ans, a[101][101];
const int dx[] = {0, -1, 0, 1, -2, -1, 0, 1, 2, -1, 0, 1, 0}, dy[] = {-2, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 2};
int main() {
	cin >> n >> m >> k;
	while (m--) {
		cin >> x >> y;
		for (int d = 0; d < 13; d++) {
			int mx = x + dx[d], my = y + dy[d];
			if (mx < 1 || mx > n || my < 0 || my > n) continue;
			a[mx][my] = 1;
		}
	}
	while (k--) {
		cin >> x >> y;
		for (int i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i > n) continue;
			for (int j = y - 2; j <= y + 2; j++) {
				if (j < 0 || j > n) continue;
				a[i][j] = 1;
			}
		}
	}
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			ans += a[i][j] == 0;
	cout << ans;
	return 0;
}
