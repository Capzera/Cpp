#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, m, r, u, v;
int dep[N], lg[N], st[N][22];
vector<int> g[N];
void dfs(int u, int fa) {
	st[u][0] = fa;
	for(int i = 1; i <= lg[dep[u]]; i++) {
		st[u][i] = st[st[u][i - 1]][i - 1];
	}
	for(auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		dfs(v, u);
	}
}
int lca(int u, int v) {
	if (dep[u] < dep[v]) swap(u, v);	
	while(dep[u] > dep[v]) {
		u = st[u][lg[dep[u] - dep[v]]];
	} 
	if(u == v) return u;	
	for(int k = lg[dep[u]]; ~k; --k) {
		if(st[u][k] == st[v][k]) continue;
		u = st[u][k], v = st[v][k];
	}  
	return st[u][0];
}
int main() {
	scanf("%d%d%d", &n, &m, &r);
	lg[0] = -1;
	for(int i = 1; i <= n; i++) lg[i] = lg[i >> 1] + 1;
	while (--n) {
		scanf("%d%d", &u, &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(r, 0);
	while (m--) {
		scanf("%d%d", &u, &v);
		printf("%d\n", lca(u, v));
	}
	return 0;
}
