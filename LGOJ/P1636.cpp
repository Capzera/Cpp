#include <bits/stdc++.h>
using namespace std;
int n, m, u, v, cnt[1001], ans;
int main() {
	cin >> n >> m;
	while (m--) {
		cin >> u >> v;
		cnt[u]++, cnt[v]++;
	}
	for (int u = 1; u <= n; u++) {
		if (cnt[u] & 1) ans++;
	}
	if (!ans) ans += 2;
	cout << ans / 2;
	return 0;
}
