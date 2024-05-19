#include <bits/stdc++.h>
using namespace std;
int main() {
	unordered_map<int, pair<int, int> > mp[2];
	int n, ans = INT_MIN, loc = -1;
	cin >> n;
	vector<int> a(n + 1);
	for (int j = 0; j < 2; j++) {
		for (int k = 1; k <= n; k++) {
			int i = k + j * (n - 2 * k + 1);
			if (!j) cin >> a[i];
			if (!mp[j].count(a[i])) {
				mp[j][a[i]] = {i, a[i]};
			} else mp[j][a[i]].second += abs(i - mp[j][a[i]].first);
		}
	}
	for (int t = 0; t < 2; t++) {
		for (auto& [x, p] : mp[t]) {
			if (p.second > ans || (p.second == ans && p.first < loc)) {
				loc = p.first;
				ans = p.second;
			}
		}
	}
	cout << ans << " " << loc;
	return 0;
}
