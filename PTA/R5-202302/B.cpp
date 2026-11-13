#include <bits/stdc++.h>
using namespace std;
long long n, dp[1001];
int main() {
	dp[1] = 1;
	cin >> n;
	for (int i = 2; i <= n; i++) {
		dp[i] = 1;
		for (int j = 1; j <= i / 2; j++) {
			dp[i] += dp[j];
		}
	}
	cout << dp[n];
	return 0;
}
