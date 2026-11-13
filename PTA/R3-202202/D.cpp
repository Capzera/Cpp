#include <bits/stdc++.h>
using namespace std;
const int N = 130;
const int dx[] = {1, 2};
int n, b[N], mp[N], mx[N], ans = INT_MAX;
char a[N];
int main() {
	cin >> n;
	for (int i = 2; i <= n; i++) mp[i] = INT_MAX;
	queue<vector<int> > q;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> b[i];
	}
	if (a[1] == 'A') {
		mp[1] = b[1];
		q.push({1, b[1], 0, b[1]});
	} else {
		q.push({1 + b[1], 0, 1, 0});
	}
	while (q.size()) {
		vector<int> vec = q.front(); q.pop();
		int x = vec[0], coin = vec[1], st = vec[2], curmax = vec[3];
		if (x == n) {
			ans = min(ans, curmax);
			continue;
		}
		for (int d = 0; d < 2; d++) {
			int nx = x + dx[d];
			if (nx > n) continue;
			if (a[nx] == 'A' && coin + b[nx] < mp[nx]) {
				mp[nx] = coin + b[nx];
				q.push({nx, coin + b[nx], 0, max(curmax, coin + b[nx])});
			} 
			if (a[nx] == 'B' && coin < mp[nx] && !st) {
				mp[nx] = coin;
				q.push({nx + b[nx], coin, 1, max(curmax, coin)});
			}
		}
	}
	cout << max(ans, 0);
	return 0;
}
