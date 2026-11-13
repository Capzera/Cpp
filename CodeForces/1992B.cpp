#include <bits/stdc++.h>
#define int long long
using namespace std;
signed main() {
	int t;
	cin >> t;
	while (t--) {
		int k, n, ans = 0;
		cin >> k >> n;
		vector<int> a(n);
		for (auto& x : a) cin >> x;
		sort(a.begin(), a.end());
		for (int i = 0; i < n - 1; i++) {
			ans += 2 * a[i] - 1;
		}
		cout << ans << endl;
	}
	return 0;
}
