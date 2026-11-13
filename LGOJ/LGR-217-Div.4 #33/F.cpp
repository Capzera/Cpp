#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
long long n, m, x, y, ans, a[1001][1001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	cin >> x >> y;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i][j] != x) continue;
			for (int d = 0; d < 4; d++) {
				int mx = i + dx[d], my = j + dy[d];
				if (mx < 1 || mx > n || my < 1 || my > m || a[mx][my] != y) continue;
				ans++;
				break;
			}
		}
	}
	cout << ans;
	return 0;
}
