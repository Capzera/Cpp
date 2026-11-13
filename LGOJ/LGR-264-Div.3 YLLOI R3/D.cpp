#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	
	int n, a, d;
	cin >> n >> a >> d;
	vector<int> b(n - 1);
	for (int i = 0; i < n - 1; ++i) cin >> b[i];
	vector<ll> c(n, 0);
	int two_d = 2 * d;
	for (int i = 1; i <= n - 1; ++i) {
		if (b[i - 1] >= 2) {
			c[i] = (b[i - 1] - 2) / (2 * d);
		}
	}
	vector<ll> block_sum;
	vector<int> block_id(n, 0);
	int last = -1, cur = 0;
	for (int i = 1; i <= n - 1; ++i) {
		if (c[i] == 0) continue;
		if (last != -1 && i - last <= 2) {
			block_sum[cur] += c[i];
			block_id[i] = cur;
		} else {
			cur++;
			block_sum.push_back(c[i]);
			block_id[i] = cur;
		}
		last = i;
	}
	
	ll ans = 0;
	if (c[a] > 0) {
		ans = block_sum[block_id[a]];
	} else {
		unordered_set<int> vis;
		for (int x = -2; x <= 2; ++x) {
			int pos = a + x;
			if (pos < 1 || pos >= n) continue;
			if (c[pos] > 0) {
				int bid = block_id[pos];
				if (vis.find(bid) == vis.end()) {
					vis.insert(bid);
					ans = max(ans, block_sum[bid]);
				}
			}
		}
	}
	cout << ans << '\n';
	return 0;
}
