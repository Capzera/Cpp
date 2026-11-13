#include <bits/stdc++.h>
using namespace std;
long long n, f[18], dp[100010];

int main() {
	cin >> n;
	for (int i = 1; i <= 17; i++) {
		f[i] = i * i * i * i;
		dp[f[i]] = 1;
	}
	memset(dp, 0x3f, sizeof(dp));
	dp[0] = 0;
	for (int i = 1; i <= 17; i++) {
		for (int j = f[i]; j <= n; j++) {
			dp[j] = min(dp[j], dp[j - f[i]] + 1);
		}
	}
	cout << dp[n];
	return 0;
}
