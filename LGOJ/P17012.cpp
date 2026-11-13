#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;
int n, dp[N], a[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		dp[i] = a[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= n; j++) {
			dp[j] = max(dp[j], dp[j - i] + a[i]);
		}
	}
	cout << dp[n];
	return 0;
}
