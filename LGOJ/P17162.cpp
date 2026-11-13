#include <bits/stdc++.h>
#include <ranges>
using namespace std;
const int N = 5e3 + 10;
long long t, n, m, W, V, C;
long long solve() {
	cin >> n >> m;
	vector<int> w, v, c;
	vector<long long> dp(m + 1);
	for (int i = 1; i <= n; i++) {
		cin >> W >> V >> C;
		if (C == 1) {
			for (int j = m; j >= W; j--) 
				dp[j] = max(dp[j], dp[j - W] + V);
		} else {
			w.push_back(W), v.push_back(V), c.push_back(C);
		}
	}
	long long ans = dp[m];
	for (auto [V, W] : std::views::zip(v, w)) {
		vector<long long> dp1 = dp;
		for (int j = W; j <= m; j++) {
			dp1[j] = max(dp1[j], dp1[j - W] + V);
		}
		ans = max(ans, dp1[m]);
	}
	return ans;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
