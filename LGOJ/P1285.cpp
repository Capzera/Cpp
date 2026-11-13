#include <bits/stdc++.h>
using namespace std;
const int N = 105;
int n, x, cnt, k = -1, mn = 1e9, col[N];
int mp[N][N], g[N][N], dp[N][N], path[N][N];
vector<int> comp[N][3];
bool dfs(int u, int c) {
	col[u] = c;
	comp[cnt][c].push_back(u);
	for (int v = 1; v <= n; v++) {
		if (!g[u][v]) continue;
		if (col[v] == c) return 0;
		if (!col[v] && !dfs(v, 3 - c)) return 0;
	}
	return 1;
}
void print(vector<int> a) {
	cout << a.size() << " ";
	for (auto& x : a) cout << x << " ";
	cout << '\n';
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		while (cin >> x && x) mp[i][x] = 1;
	}
	for (int i = 1; i <= n; i++)
		for (int j = i + 1; j <= n; j++)
			if (!mp[i][j] || !mp[j][i])
				g[i][j] = g[j][i] = 1;
	for (int i = 1; i <= n; i++) {
		if (col[i]) continue;
		cnt++;
		if (!dfs(i, 1)) return puts("No solution"), 0;
	}
	dp[0][0] = 1;
	for (int i = 1; i <= cnt; i++) {
		for (int j = 0; j <= n; j++) {
			for (int p = 1; p <= 2; p++) {
				int w = comp[i][p].size();
				if (j >= w && dp[i - 1][j - w]) {
					dp[i][j] = 1;
					path[i][j] = p;
				}
			}
		}
	}
	for (int j = 0; j <= n; j++) {
		if (!dp[cnt][j]) continue;
		int diff = abs(j - (n - j));
		if (diff < mn) {
			mn = diff;
			k = j;
		}
	}
	if (k == -1) return puts("No solution"), 0;
	vector<int> t1, t2;
	for (int i = cnt; i >= 1; --i) {
		for (int x : comp[i][path[i][k]]) t1.push_back(x);
		for (int x : comp[i][3 - path[i][k]]) t2.push_back(x);
		k -= comp[i][path[i][k]].size();
	}
	sort(t1.begin(), t1.end()), sort(t2.begin(), t2.end());
	print(t1), print(t2);
	return 0;
}
