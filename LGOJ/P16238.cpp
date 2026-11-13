#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 2e5 + 10;
int n, x, ans, mx, a[N], sum[N];
map<int, vector<int> > mp;
signed main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> x, a[i] = x - a[i];
	for (int i = 1; i <= n; i++) {
		sum[i] = sum[i - 1];
		if (!a[i]) {
			ans++, sum[i]++;
		} else {
			mp[a[i]].push_back(i);
		}
	}
	for (auto& [x, id] : mp) {
		int mn = 0;
		for (int j = 1; j <= id.size(); j++) {
			int k = id[j - 1], cnt = j - sum[k];
			mn = min(mn, cnt - 1);
			mx = max(mx, cnt - mn);
		} 
	}
	cout << ans + mx;
	return 0;
}
