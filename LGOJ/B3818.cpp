#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int  n, m, r, c, ans;
char a[501][501];
int main() {
	cin >> n >> m >> r >> c;
	for (int i = 1; i <= n; i++) 
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i][j] == '.') continue;
			int cnt = 0;
			for (int d = 0; d < 4; d++) {
				bool f = 1;
				for (int len = 0; ; len++) {
					int x = i + dx[d] * len, y = j + dy[d] * len;
					if (x < 1 || x > n || y < 1 || y > m) break;
					if (a[x][y] != '.' && a[x][y] != a[i][j]) {
						f = 0;
						break;
					}
				}
				cnt += f;
			}
			ans += cnt > 2;
		}
	}
	cout << ans;
	return 0;
}
