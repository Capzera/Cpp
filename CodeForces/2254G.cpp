#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;
int t, n, ans, pa[N], ind[N], id[N];
long long w[N], sum[N];
priority_queue<long long, vector<long long>, greater<long long> > pq[N];
void solve() {
	vector<long long> c;
	long long tot = 0, leaf = 0;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> w[i];
		tot += w[i];
		ind[i] = 0;
		id[i] = i;
		while (pq[i].size()) pq[i].pop();
	}
	for (int i = 2; i <= n; i++) {
		cin >> pa[i];
		ind[pa[i]]++;
	}
	for (int i = 1; i <= n; i++) {
		if (ind[i]) continue;
		leaf++;
	}
	for (int i = n; i >= 1; i--) {
		int u = id[i];
		if (!ind[i]) {
			pq[u].push(w[i]);
		} else {
			long long cost = pq[u].top(); pq[u].pop();
			c.push_back(min(w[i], cost));
			pq[u].push(max(w[i], cost));
		}
		if (i == 1) continue;
		int fa = id[pa[i]];
		if (pq[fa].size() < pq[u].size()) {
			swap(fa, u), swap(id[pa[i]], id[i]);
		}
		while (pq[u].size()) {
			pq[fa].push(pq[u].top()); pq[u].pop();
		}
	}
	sort(c.begin(), c.end());
	sum[0] = 0;
	for (int i = 1; i <= c.size(); i++) {
		sum[i] = sum[i - 1] + c[i - 1];
	}
	for (int k = 1; k <= n; k++) {
		if (k < leaf) {
			cout << -1 << ' ';
		} else {
			cout << tot - sum[n - k] << ' ';
		}
	}
	cout << '\n';
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) solve();
	return 0;
}
/*
13
8
1 1 3 3 2 2 3 2
1 1 2 1 1 5 4
8
4 5 4 4 6 2 7 6
1 1 3 4 2 6 6
3
1 2 2
1 2
6
5 3 3 2 4 2
1 1 2 4 1
4
1 2 1 3
1 1 1
6
4 2 4 2 4 4
1 2 2 4 2
3
1 2 1
1 2
3
1 1 1
1 2
3
2 2 3
1 2
6
5 5 2 1 3 1
1 1 1 2 1
7
3 1 3 1 3 3 2
1 1 1 2 2 1
5
1 1 1 1 1
1 1 1 2
8
3 7 8 2 8 5 1 3
1 2 3 2 3 6 5
*/
