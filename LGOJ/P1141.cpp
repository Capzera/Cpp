#include <bits/stdc++.h>
#define pii pair<int, int> 
#if defined(_WIN32) || defined(_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#endif
using namespace std;
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar_unlocked();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar_unlocked();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - 48;
		ch = getchar_unlocked();
	}
	return ans * f;
}
inline int cread() {
	int ch = getchar_unlocked();
	while (isspace(ch)) ch = getchar_unlocked();
	return ch;
}
inline string sread() {
	string ans;
	char ch = cread();
	if (ch == EOF) return "";
	do {
		ans += ch;
		ch = getchar_unlocked();
	} while (!isspace(ch) && ch != EOF);
	return ans;
}
inline void write(long long x) {
	if (x < 0) putchar_unlocked('-'), write(-x);
	if (x > 9) write(x / 10);
	putchar_unlocked(x % 10 + '0');
}
const int N = 1e3 + 5, dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, m, x, y, g[N][N];
bool vis[N][N];
char a[N][N];
void bfs(int x, int y) {
	queue<pii> q;
	vector<pii> vec;
	q.push({x, y});
	vec.push_back({x, y});
	vis[x][y] = 1;
	while (q.size()) {
		auto& [x, y] = q.front(); q.pop();
		for (int d = 0; d < 4; d++) {
			int nx = x + dx[d], ny = y + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > n || g[nx][ny] || a[nx][ny] == a[x][y] || vis[nx][ny]) continue;
			q.push({nx, ny});
			vec.push_back({nx, ny});
			vis[nx][ny] = 1;
		}
	}
	for (auto& p : vec) {
		g[p.first][p.second] = vec.size();
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	n = read(), m = read();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			a[i][j] = cread();
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (g[i][j]) continue;
			bfs(i, j);
		}
	}
	while (m--) {
		x = read(), y = read();
		write(g[x][y]);
		putchar_unlocked('\n');
	}
	return 0;
}
