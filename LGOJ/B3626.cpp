#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 5;
int n, a[N], dx[] = {-1, 1, 0};
queue<int> q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	q.push(1);
	while (q.size()) {
		dx[2] = q.front(); q.pop();
		if (dx[2] == n) {
			cout << a[n];
			return 0;
		}
		for (int d = 0; d < 3; d++) {
			int nx = dx[2] + dx[d];
			if (nx <= 1 || nx > n || a[nx]) continue;
			a[nx] = a[dx[2]] + 1;
			q.push(nx);
		}
	}
	return 0;
}

