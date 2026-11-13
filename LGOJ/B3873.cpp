#include <bits/stdc++.h>
using namespace std;
int n, m, w, v;
int main() {
	cin >> n >> m;
	vector<int> dp(m + 1, 1e9);
	dp[0] = 0;
	for (int i = 0; i < n; i++) {
		cin >> v >> w;
		for (int j = m; j >= 0; j--) {
			dp[j] = min(dp[j], dp[max(j - w, 0)] + v);
		}
	}
	if (dp[m] == 1e9) cout << "no solution";
	else cout << dp[m];
	return 0;
}
