#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, p = 19650827;
long long n, a[N], dp[N][N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		dp[i][i] = 1;
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i <= n - len + 1; i++) {
			int j = i + len - 1;
			if (a[i] < a[i + 1] || a[i] < a[j]) dp[i][j] = (dp[i][j] + dp[i + 1][j]) % p;
			if (a[j] > a[j - 1] || a[j] > a[i]) dp[i][j] = (dp[i][j] + dp[i][j - 1]) % p;
		}
	}
	cout << dp[1][n];
	return 0;
}
