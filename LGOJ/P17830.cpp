#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 5, M = 3e5 + 5;
int n, m, a[N], pa[N], lst[N], sz[N];
int mp[M];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	// freopen("card.in", "r", stdin);
	// freopen("card.out", "w", stdout);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sz[i] = 1;
	}
	for (int i = n; i; i--) {
		int v = a[i];
		pa[i] = i + 1;
		if (!mp[v]) {
			lst[v] = i;
		} else {
			pa[i] = mp[v];
		}
		mp[v] = i;
	}
	for (int i = 1; i <= n; i++) sz[pa[i]] += sz[i];
	for (int i = 1; i <= m; i++) cout << sz[lst[i]] << " ";
	return 0;
}
