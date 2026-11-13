#include <bits/stdc++.h>
using namespace std;
long long n, m, k, t, a, x, y;
int main() {
	cin >> n >> m >> k >> t;
	vector<vector<vector<int> > > grid(n + 1, vector<vector<int> >(m + 1));
	vector<vector<priority_queue<int, vector<int>, greater<int> > > > mp(n + 1, vector<priority_queue<int, vector<int>, greater<int> > > (m + 1));
	for (int i = 1; i <= t; i++) {
		cin >> a >> x >> y;
		if (mp[x][y].size() < k) {
			cout << -1 << endl;
			mp[x][y].push(a);
			grid[x][y].push_back(a);
			continue;
		}
		int mn = mp[x][y].top(), j = k - 1, sum = 0; mp[x][y].pop();
		for (; j > 0; j--) {
			if (grid[x][y][j] == mn) break;
			sum++;
		}
		for (; j < k - 1; j++) grid[x][y][j] = grid[x][y][j + 1];
		mp[x][y].push(a);
		grid[x][y][j] = a;
		cout << mn << " " << sum << endl;
	}
	return 0;
}
