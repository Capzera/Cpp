#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int t, n, x, mx, a[N], dx[] = {-1, 1, 0};
int solve() {
	cin >> x >> n;
	mx = max(x, n) + 1;
	if (x == n) return 0;
	memset(a, 0, sizeof a);
	queue<int> q;
	q.push(x);
	while (q.size()) {
		dx[2] = q.front(); q.pop();
		if (dx[2] == n) return a[n];
		for (int d = 0; d < 3; d++) {
			int nx = dx[2] + dx[d];
			if (nx < 1 || nx == x || nx > mx || a[nx]) continue;
			a[nx] = a[dx[2]] + 1;
			q.push(nx);
		}
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
