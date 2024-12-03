#include <bits/stdc++.h>
using namespace std;
int n, a[1001], dp[1001], ans = 0;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		a[i] += a[i - 1];
	}
	if (a[n] < 0) {
		cout << "Impossible";
		return 0;
	}
	for (int i = 1; i <= n; i++) {
		if (a[i] < 0) continue;
		for (int j = 0; j < i; j++) {
			if (a[i] - a[j] < 0) continue;
			dp[i] = max(dp[i], dp[j] + 1);
		}
	}
	cout << dp[n];
	return 0;
}
