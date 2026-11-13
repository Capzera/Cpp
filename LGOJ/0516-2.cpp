#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int t, n, m, k, x, y, d;
char a[N][N];
void solve() {
	scanf("%d%d%d%d%d%d", &n, &m, &k, &x, &y, &d);
	for (int i = 1; i <= n; i++) scanf("%s", a[i] + 1);
	int ans = 1;
	a[x][y] = 'a';
	while (k--) {
		int nx = x + dx[d], ny = y + dy[d];
		if (nx < 1 || nx > n || ny < 1 || ny > m || a[nx][ny] == 'x') {
			d = (d + 1) % 4;
			continue;
		}
		x = nx, y = ny;
		if (a[x][y] != 'a') {
			ans++;
			a[x][y] = 'a';
		}
	}
	cout << ans << '\n';
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
