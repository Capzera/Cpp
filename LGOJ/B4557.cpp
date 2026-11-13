#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, 0, -1, 0, -1, -1, 1, 1}, dy[] = {0, 1, 0, -1, -1, 1, -1, 1};
int n, m, k, x, y;
char a[1001][1001];
int main() {
	memset(a, 0x30, sizeof a);
	cin >> n >> m >> k;
	while (k--) {
		cin >> x >> y;
		a[x][y] = '*';
	}
	for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
		if (a[i][j] != '*') continue;
		for (int d = 0; d < 8; d++) {
			x = i + dx[d], y = j + dy[d];
			if (x < 1 || x > n || y < 1 || y > m || a[x][y] == '*') continue;
			a[x][y]++;
		}
	}
	for (int i = 1; i <= n; i++)  {
		for (int j = 1; j <= m; j++) cout << a[i][j] << " ";
		puts("");
	}
	return 0;
}
