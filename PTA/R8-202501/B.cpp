#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
string ori, u, v;
long long n, w, tot, ans, score;
map<string, int> mp;
vector<pii> g[201];
void dfs(int u, long long sum) {
	ans = max(ans, sum);
	for (int i = 0; i < g[u].size(); i++) {
		int v = g[u][i].first, w = g[u][i].second;
		dfs(v, sum + w);
	}
}
int main() {
	cin >> ori >> score >> n;
	mp[ori] = ++tot;
	for (int i = 1; i <= n; i++) {
		cin >> u >> v >> w;
		if (!mp.count(u)) mp[u] = ++tot;
		if (!mp.count(v)) mp[v] = ++tot;
		g[mp[u]].push_back({mp[v], w});
	}
	dfs(1, score);
	cout << ans;
	return 0;
}
/*
X01 20
4
X01 Y02 20
Y02 Z03 15
A01 B02 10
Z03 C04 
*/
