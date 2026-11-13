#include<bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, m, u, v, sum;
int cnt[N], dp[N];
vector<int> vec, g[N];
queue <int> q;
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		cnt[v]++;
	}
	for (int i = 1; i <= n; i++) {
		dp[i] = 1;
		if (cnt[i]) continue;
		q.push(i);
		vec.push_back(i);
	}
	while (q.size()) {
		int u = q.front(); q.pop();
		for (auto& v : g[u]) {
			if (!--cnt[v]) {
				q.push(v);
				vec.push_back(v);
			}
		}
	}
	for (auto& u : vec)
		for (auto& v : g[u])
			dp[v] = max(dp[v], dp[u] + 1);
	for (int i = 1; i <= n; i++) printf("%d\n", dp[i]);
	return 0;
}
