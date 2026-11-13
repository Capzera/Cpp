#include <bits/stdc++.h>
#define ll long long
#define pii pair<int, ll>
using namespace std;
const int N = 2e5 + 10;
const ll inf = 1 << 60;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return f * ans;
}
int n, u, v;
void solve() {
	n = read();
	vector<int> g[n + 1];
	for (int i = 1; i < n; i++) {
		u = read(), v = read();
		g[u].push_back(v), g[v].push_back(u);
	}
	if (n == 2) {
		cout << 0 << '\n';
		return;
	}
	int ans = 0, mx = 0;
	for (int u = 1; u <= n; u++) {
		ans += g[u].size() == 1;
	}
	for (int u = 1; u <= n; u++) {
		int cnt = 0;
		for (auto& v : g[u]) {
			cnt += g[v].size() == 1;
		}
		mx = max(mx, cnt);
	}
	cout << ans - mx << '\n';
}

int main() {
	int t = 1;
	t = read();
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
