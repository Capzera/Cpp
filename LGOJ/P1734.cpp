#include <bits/stdc++.h>
using namespace std;
int n, fac[1001], dp[1001];
int main() {
	cin >> n;
	for (int i = 2; i <= n; i++)
		for (int j = 1; j < i; j++)
			if (i % j == 0) fac[i] += j;
	for (int i = 1; i <= n; i++) {
		for (int j = n; j >= i; j--) {
			dp[j] = max(dp[j], dp[j - i] + fac[i]);
		}
	}
	cout << dp[n];
	return 0;
}
