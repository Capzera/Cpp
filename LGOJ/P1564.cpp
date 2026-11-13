#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, x;
	cin >> n >> m;
	vector<int> sum(n + 1), dp(n + 1, 2500);
	dp[0] = 0;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		sum[i] = sum[i - 1] + (x == 1 ? 1 :-1);
		for (int j = 0; j < i; j++) {
			if (abs(sum[i] - sum[j]) == i - j  || abs(sum[i] - sum[j]) <= m) {
				dp[i] = min(dp[i], dp[j] + 1);
			}
		}
	}
	cout << dp[n];
	return 0;
}
