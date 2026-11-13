#include <bits/stdc++.h>
using namespace std;
const int N = 1e2 + 10;
long long n, a[N], dp[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 2; i <= n; i++) {
		dp[i] = INT_MAX;
		for (int j = 1; j < i; j++) {
			if (j + a[j] < i) continue;
			dp[i] = min(dp[i], dp[j] + 1);
		}
	}
	cout << dp[n];
	return 0;
}
