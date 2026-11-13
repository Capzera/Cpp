#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, m, k, u, v, ans, tot, mx, id, t[N], cnt[N];
vector<int> a[N];
int main() {
	cin >> m >> n >> k;
	for (int i = 1; i <= n; i++) cin >> t[i];
	for (int i = 1; i <= n; i++) cin >> v, a[t[i]].push_back(v);
	for (int i = 1; i <= m; i++) {
		sort(a[i].begin(), a[i].end(), greater<int>());
		long long sum = 0;
		for (auto& j : a[i]) {
			sum += j;
			cnt[i]++;
			if (sum >= k) break;
		}
		if (sum < k) return puts("-1"), 0;
		tot += cnt[i];
		if (cnt[i] > mx) {
			mx = cnt[i];
			id = i;
		}
	}
	if (tot - mx >= mx - 1) {
		cout << tot;
		return 0;
	}
	int rest = 0;
	for (int i = 1; i <= n; i++) {
		if (i != id) rest += a[i].size() - cnt[i];
	}
	cout << (tot - mx + rest >= mx - 1 ? 2ll * mx - 1 : -1);
	return 0;
}
