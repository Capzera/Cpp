#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 5;
int n, k, u, v;
vector<int> g[N];
int check(int R) {
	
	if (n <= 4000) {
		for (int u = 2; u <= n; u++) {
			
		}
		return 0;
	}
	return 0;
}
void solve() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	int l = 0, r = n + 1, ans = n;
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = r = mid;
		} else l = mid + 1;
	}
	cout << ans << '\n';
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int c, t;
	cin >> c >> t;
	while (t--) solve();
	return 0;
}
