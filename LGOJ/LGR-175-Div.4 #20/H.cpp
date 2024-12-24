#include <bits/stdc++.h>
using namespace std;
long long T, n, t, m, x, def[5001];
void solve() {
	cin >> n >> t >> m;
	priority_queue<int> pq;
	for (int i = 1; i <= n; i++) cin >> def[i];
	for (int i = 1; i <= m; i++) {
		cin >> x;
		def[x] -= 2;
	}
	for (int i = 1; i <= n; i++) {
		if (def[i] <= 0) pq.push(def[i]);
	}
	while (pq.size() && t >= 1 - pq.top()) {
		t -= 1 - pq.top();
		pq.pop();
	}
	cout << n - pq.size() << endl;
}
int main() {
	cin >> T;
	while (T--) {
		solve();
	}
	return 0;
}
/*

0 -1 -1 3
*/
