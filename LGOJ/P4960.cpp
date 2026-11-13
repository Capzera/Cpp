#include <bits/stdc++.h>
using namespace std;
map<int, int> mp;
int n, x, mx;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		mp[x]++;
		mx = max(mx, mp[x]);
	}
	if (mp.size() < mx) {
		cout << mp.size() << " " << 1 << '\n';
		for (auto&& [x, y] : mp) {
			cout << y << ' ';
			while (y--) cout << x << " ";
			cout << '\n';
		}
	} else {
		cout << mx << " " << 2 << '\n';
		vector<int> ans[mx + 1];
		for (auto&& [x, y] : mp) {
			for (int i = 0; i < y; i++) ans[i].push_back(x);
		}
		for (int i = 0; i < mx; i++) {
			cout << ans[i].size() << " ";
			for (auto& x : ans[i]) cout << x << " ";
			cout << '\n';
		}
	}
	return 0;
}
