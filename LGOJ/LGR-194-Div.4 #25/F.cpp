#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m, ans, x;
int a[105][105];
int vis[1000];
void dfs(int sn, int fa) {
	vis[sn] = 1;
	for (int fn = 1; fn <= n; fn++) {
		if (fn == fa || vis[fn] || !a[sn][fn]) continue;
		a[fa][fn] = 1;
		dfs(fn, fa);
	}
}
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> m;
		while (m--) {
			cin >> x;
			a[i][x] = 1;
		}
	}
	for (int i = 1; i <= n; i++) {
		memset(vis, 0, sizeof(vis));
		dfs(i, i);
	}
	for (int i = 1; i <= n; i++) {
		
		cout << accumulate(a[i] + 1, a[i] + n + 1, 0ll) << " ";
	}
	return 0;
}
