#include <bits/stdc++.h>
using namespace std;
int n, m, k, op, u, v, mp[5001];
int main() {
	cin >> n >> m >> k;
	set<int> pro[n];
	for (int j = 1; j <= m; j++) pro[0].insert(j);
	while (k--) {
		scanf("%d%d%d", &op, &u, &v);
		if (op == 1) {
			pro[mp[u]].erase(u);
			pro[v].insert(u);
			mp[u] = v;
		} else {
			if (pro[v].size() < u) {
				puts("-1");
				continue;
			}
			auto it = pro[v].begin();
			advance(it, u - 1);
			cout << *it << '\n';
		}
	}
	return 0;
}
