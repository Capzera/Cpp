#include <bits/stdc++.h>
using namespace std;
int W, n, a[16], b[16], t[1 << 16], w[1 << 16], dp[1 << 16];  
int main() {
	cin >> W >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i] >> b[i];
	}
	for (int i = 1; i < 1 << n; i++) {
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			t[i] = max(t[i], a[j]);
			w[i] += b[j];
		}
	}
	memset(dp, 0x3f, sizeof(dp));
	dp[0] = 0;
	for (int i = 0; i < 1 << n; i++)
		for (int j = i; j; j = i & (j - 1))
			if (w[j] <= W) dp[i] = min(dp[i], dp[i ^ j] + t[j]);
	cout << dp[(1 << n) - 1];
	return 0;
}
