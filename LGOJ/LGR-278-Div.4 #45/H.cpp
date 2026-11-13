#include <bits/stdc++.h>
using namespace std;
const int N = 301;
int n, m, k, v, vis[N];
vector<int> g[N];
string solve() {
	cin >> k;
	vector<int> id(k), score(k);
	for (auto& x : id) cin >> x;
	for (auto& x : score) cin >> x;
	for (auto& u : id) {
		if (vis[u] == 2) return "Error";
		for (auto& v : g[u]) {
			if (vis[v] != 2) return "Error";
		}
	}
	string ans;
	for (int i = 0; i < k; i++) {
		ans += score[i] >= 60 ? "P" : "F";
		vis[id[i]] = 1 + (score[i] >= 60);
	}
	return ans;
}
int main() {
	cin >> n >> m;
	for (int u = 1; u <= n; u++) {
		cin >> k;
		while (k--) {
			cin >> v;
			g[u].push_back(v);
		}
	}
	while (m--) cout << solve() << '\n';
	return 0;
}
