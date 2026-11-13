#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	Trie* child[2] = {nullptr, nullptr};
	Trie() : val(0) {}
} *root;
void build(int val) {
	auto p = root;
	for (int j = 31; ~j; j--) {
		int bit = val >> j & 1;
		p = p->child[bit] ? p->child[bit] : p->child[bit] = new Trie();
	}
}
int query(int val) {
	auto p = root;
	int mask = 0;
	for (int j = 31; ~j; j--) {
		int bit = val >> j & 1;
		if (p->child[bit ^ 1]) {
			p = p->child[bit ^ 1];
			mask ^= 1 << j;
		} else p = p->child[bit];
	}
	return mask;
}
int n, m, u, v, w, ans, sum[100001];
vector<pair<int, int> > edge[100001];
void dfs(int u, int fa) {
	for (auto& [v, w] : edge[u]) {
		if (v == fa) continue;
		sum[v] = sum[u] ^ w;
		dfs(v, u);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		edge[u].push_back({v, w});
		edge[v].push_back({u, w});
	}
	dfs(1, -1);
	root = new Trie();
	for (int i = 1; i <= n; i++) build(sum[i]);
	for (int i = 1; i <= n; i++) ans = max(ans, query(sum[i]));
	cout << ans;
	return 0;
}
