#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, m, ans, mx;
char ch, a[1001][1001];
int check(int x, int y) {
	if (a[x][y] == '#') return 0;
	for (int d = 0; d < 4; d++) {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 1 || mx > n || my < 1 || my > m) continue;
		if (a[mx][my] == '#') return 0;
	}
	return 1;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			ans += check(i, j);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i][j] == '.') continue;
			a[i][j] = '.';
			int sum = check(i, j);
			for (int d = 0; d < 4; d++) {
				int mx = i + dx[d], my = j + dy[d];
				if (mx < 1 || mx > n || my < 1 || my > m) continue;
				sum += check(mx, my);
			}
			a[i][j] = '#';
			mx = max(mx, sum);
		}
	}
	cout << ans + mx;
	return 0;
}
