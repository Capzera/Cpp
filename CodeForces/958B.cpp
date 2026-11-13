#include <bits/stdc++.h>
using namespace std;
int n, u, v, cnt;
int main() {
	cin >> n;
	vector<int> g(n + 1);
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u]++, g[v]++;
	}
	for (int u = 1; u <= n; u++) {
		cnt += g[u] == 1;
	}
	cout << cnt;
	return 0;
}
