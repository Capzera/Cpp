#include <bits/stdc++.h>
using namespace std;
vector<int> g[27];
int n, m, u, v, cnt[27], mp[703], vis[27];
set<int> node;
string ans;
bool dfs(int sn) {
	if (vis[sn]) return 0;
	for (auto& v : g[sn]) {
		if (!dfs(v)) return 0;
	}
	vis[sn] = 1;
	return 1;
}

int dfs2(int sn, int depth) {
	if (depth == n) return 1;
	ans += sn + 'A' - 1;
	for (auto& v : g[sn]) {
		if (dfs2(v, depth + 1)) return 1;
	}
	return 0;
}

int main() {
	cin >> n >> m;
	for (int idx = 1; idx <= m; idx++) {
		char x, ch, y;
		cin >> x >> ch >> y;
		u = x - 'A' + 1, v = y - 'A' + 1;
		if (mp[u * 26 + v]) continue;
		g[u].push_back(v);
		node.insert(u), node.insert(v);
		cnt[v]++;
		mp[u * 26 + v] = 1;
		for (int u = 1; u <= n; u++) {
			if (node.count(u) && !cnt[u]) {
				memset(vis, 0, sizeof(vis));
				if (!dfs(u)) {
					cout << "Inconsistency found after " << idx << " relations.";
					return 0;
				}
				memset(vis, 0, sizeof(vis));
				if (dfs2(u, 0)) {
					cout << "Sorted sequence determined after " << idx << " relations: ";
					cout << ans << ".";
					return 0;
				}
			}
		}
	}
	cout << "Sorted sequence cannot be determined.";
	return 0;
}
