#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;
vector<int> g[N];
int n, l, r, dep[N];
void dfs(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		dfs(v, u);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> l >> r;
		if (l) g[i].push_back(l);
		if (r) g[i].push_back(r);
	}
	dep[1] = 1;
	dfs(1, -1);
	cout << *max_element(dep + 1, dep + n + 1);
	return 0;
}

