#include <bits/stdc++.h>
using namespace std;
long long n, t, a[1001], sum[1001], dp[1001];
int main() {
	cin >> n >> t;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum[i] = sum[i - 1] + a[i];
	}
	for (int i = 1; i <= n; i++) {
		dp[i] = a[i] * n * n + dp[i - 1];
		for (int j = 1; j < i; j++) {
			if (a[i] + a[j] > t) continue;
			dp[i] = min(dp[i], dp[j - 1] + (a[i] + a[j]) * (sum[i] - sum[j - 1]));
		}
	}
	cout << dp[n];
	return 0;
}
