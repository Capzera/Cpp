#include <bits/stdc++.h>
using namespace std;
long long n, m;
int solve() {
	cin >> n >> m;
	vector<vector<int> > a(n, vector<int>(m));
	vector<pair<int, int> > vec;
	int mx = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> a[i][j];
			mx = max(mx, a[i][j]);
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (a[i][j] != mx) continue;
			vec.push_back({i, j});
		}
	}
	bool fr = 1, fc = 1;
	int r = -1, c = -1;
	for (auto& [x, y] : vec) {
		if (x != vec[0].first) {
			if (c == -1) c = y;
			else if (c != y) fc = 0;
		}
		if (y != vec[0].second) {
			if (r == -1) r = x;
			else if (r != x) fr = 0;
		}
	}
	return mx - (fr || fc);
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) cout << solve() << endl;
	return 0;
}
