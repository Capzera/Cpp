#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, x, l;
	long long ans = 0;
	cin >> n >> m;
	vector<unordered_set<int>> mp(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> x;
			mp[i].insert(x);
		}
	}
	for (int i = 0; i < n; i++) {
		cin >> l;
		while (l--) {
			cin >> x;
			if (mp[i].count(x)) mp[i].erase(x);
		}
		ans += 2 * mp[i].empty();
	}
	cout << ans << endl;
	return 0;
}
