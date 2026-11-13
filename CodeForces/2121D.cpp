#include <bits/stdc++.h>
using namespace std;
int n, a[41], b[41];

void solve() {
	cin >> n;
	vector<pair<int, int> > vec;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = 1; i < n; i++) {
		bool f = 1;
		for (int j = 1; j <= n - i; j++) {
			if (a[j] > a[j + 1]) {
				swap(a[j], a[j + 1]);
				vec.push_back({1, j});
				f = 0;
			}
		}
		if (f) break;
	}
	for (int i = 1; i < n; i++) {
		bool f = 1;
		for (int j = 1; j <= n - i; j++) {
			if (b[j] > b[j + 1]) {
				swap(b[j], b[j + 1]);
				vec.push_back({2, j});
				f = 0;
			}
		}
		if (f) break;
	}
	for (int i = 1; i <= n; i++) {
		if (a[i] > b[i]) vec.push_back({3, i});
	}
	cout << vec.size() << endl;
	for (auto& [x, y] : vec) cout << x << " " << y << endl;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) solve();
	return 0;
}
/*
1
20
40 38 36 34 32 30 28 26 24 22 20 18 16 14 12 10 8 6 4 2
39 37 35 33 31 29 27 25 23 21 19 17 15 13 11 9 7 5 3 1
*/
