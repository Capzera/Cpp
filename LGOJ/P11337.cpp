#include <bits/stdc++.h>
using namespace std;
int T, n, P, Q;
int solve() { // GESP 2024120701 dp[j] 表示花费为j时的最大武器强度
	cin >> n >> P >> Q;
	vector<int> p(n), c(n), dp(Q + 1);
	for (int i = 0; i < n; i++) cin >> p[i] >> c[i];
	for (int i = 0; i < n; i++)
		for (int j = Q; j >= c[i]; j--)
			dp[j] = max(dp[j], dp[j - c[i]] + p[i]);
	for (int j = 1; j <= Q; j++) 
		if (dp[j] >= P) return j;
	return -1;
}
int main() {
	cin >> T;
	while (T--) cout << solve() << endl;
	return 0;
}
