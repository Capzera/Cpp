#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, q, u, v, idx, cnt[N];
vector<int> g[N];
string s;
void dfs(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		cnt[v] += cnt[u];
		dfs(v, u);
	}
}
int main() {
	cin >> n;
	for (int v = 2; v <= n; v++) {
		cin >> u;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	cin >> s >> q;
	while (q--) {
		cin >> u;
		cnt[u]++;
	}
	dfs(1, -1);
	for (int i = 1; i <= n; i++) {
		cout << ((s[i - 1] - '0') ^ (cnt[i] & 1));
	}
	return 0;
}
