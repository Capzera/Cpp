#include <bits/stdc++.h>
using namespace std;
long long n;
int main() {
	cin >> n;
	long long sum = n * (n + 1) / 2;
	if (sum & 1) return puts("0"), 0;
	sum /= 2;
	vector<long long> dp(sum + 1);
	dp[0] = 1;
	for (int i = 1; i <= n; i++) {
		for (int j = sum; j >= i; j--) {
			dp[j] = dp[j] + dp[j - i];
		}
	}
	cout << dp[sum] / 2;
	return 0;
}
