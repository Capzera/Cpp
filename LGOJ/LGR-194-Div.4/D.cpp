#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m;
long long ans;
map<int, int> mp, cnt;
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	vector<int> a(m + 1), b(n + 1);
	vector<vector<int> > vec;
	for (int i = 1; i <= m; i++) {
		cin >> a[i];
		mp[i] = a[i];
	}
	for (int i = 1; i <= n; i++) {
		cin >> b[i];
		cnt[b[i]]++;
	}
	for (auto& [lan, time] : cnt) {
		vec.push_back({lan, mp[lan] * time});
	}
	sort(vec.begin(), vec.end(), [&](auto& x, auto& y) {
		if (x[1] != y[1]) return x[1] > y[1];
		return x[0] < y[0];
	});
	cout << vec[0][0];
	return 0;
}
