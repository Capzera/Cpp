#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
char s[N][N];
int n, m, ans, cnt, r1, r2, c1, c2;
void dfs(int x, int y) {
	s[x][y] = '.';
	cnt++;
	r1 = min(r1, x), c1 = min(c1, y), r2 = max(r2, x), c2 = max(c2, y);
	for (int d = 0; d < 4; d++) {
		int nx = x + dx[d], ny = y + dy[d];
		if (nx < 1 || nx > n || ny < 1 || ny > m || s[nx][ny] != '#') continue;
		dfs(nx, ny);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) 
		cin >> s[i][j];
	for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
		if (s[i][j] == '.') continue;
		ans++;
		r1 = n + 1, c1 = m + 1, r2 = 0, c2 = 0, cnt = 0;
		dfs(i, j);
		if (cnt != (r2 - r1 + 1) * (c2 - c1 + 1)) return puts("Bad placement."), 0; 
	}
	printf("There are %d ships.", ans);
	return 0;
}

