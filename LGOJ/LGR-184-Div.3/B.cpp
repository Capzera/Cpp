#include <bits/stdc++.h>
using namespace std;
void solve() {
	int n;
	cin >> n;
	vector<int> a(n);
	unordered_map<int, int> mp;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		mp[a[i]]++;
	}
	sort(a.begin(), a.end());
	for (int i = 1; i < n - 1; i++) {
		int target = a[n - 1] - a[i] + a[0];
		if (!mp.count(target)) continue;
		int j = lower_bound(a.begin(), a.end(), target) - a.begin();
		if (j == n - 1 || ((j == 0 || j == i) && mp[target] == 1)) continue;
		cout << "Yes" << endl;
		return;
	}
	cout << "No" << endl;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int t, id;
	cin >> t >> id;
	while (t--) {
		solve();
	}
	return 0;
}
/*

*/
