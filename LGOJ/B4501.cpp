#include <bits/stdc++.h>
using namespace std;
const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
int n, m, ans, a[101][101];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			int f = 1;
			for (int d = 0; d < 8; d++) {
				int x = i + dx[d], y = j + dy[d];
				if (x < 1 || x > n || y < 1 || y > m) continue;
				if (a[x][y] < a[i][j]) {
					f = 0;
					break;
				}
			}
			ans += f;
		}
	}
	cout << ans;
	return 0;
}
