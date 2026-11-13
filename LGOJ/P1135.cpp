#include <bits/stdc++.h>
using namespace std;
const int N = 201;
int n, s, t, a[N], ans[N];
queue<int> q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> s >> t;
	for (int i = 1; i <= n; i++) cin >> a[i];
	q.push(s);
	while (q.size()) {
		int u = q.front(); q.pop();
		if (u == t) {
			cout << ans[u];
			return 0;
		}
		for (int d = -1; d <= 1; d += 2) {
			int v = u + d * a[u];
			if (v < 0 || v > n || ans[v] || v == s) continue;
			ans[v] = ans[u] + 1;
			q.push(v);
		}
	}
	cout << -1;
	return 0;
}
