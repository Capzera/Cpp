#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10;
vector<int> a[N];
int n, m, s, t, ans, dp[N];
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> s >> t;
		a[s].push_back(t);
	}
	for (int i = m; i > 0; i--) {
		if (a[i].empty()) {
			dp[i] = dp[i + 1] + 1;
			continue;
		}
		for (auto& t : a[i]) {
			dp[i] = max(dp[i], dp[i + t]);
		}
	}
	cout << dp[1];
	return 0;
}
