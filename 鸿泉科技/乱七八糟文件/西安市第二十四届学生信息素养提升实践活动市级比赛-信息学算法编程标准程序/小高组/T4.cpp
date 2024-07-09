/*
吃饭 动态规划  同初中组T2
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<long long> a(n + 1);
	long long ans = 0;
	for (int i = 1; i <= n; i++) cin >> a[i];
	vector<long long> dp = a;
	for (int i = 2; i <= n; i++) {
		for (int j = 1; j <= sqrt(i); j++) {
			if (i % j) continue;
			dp[i] = max(dp[i], dp[j] + a[i]);
			if (j > 1) dp[i] = max(dp[i], dp[i / j] + a[i]);
		}
		ans = max(ans, dp[i]);
	}
	cout << ans << endl;
	return 0;
}
