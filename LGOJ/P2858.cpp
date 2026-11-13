#include <bits/stdc++.h>
using namespace std;
const int N = 2010;
long long n, a[N], dp[N][N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		dp[i][i] = a[i] * n;
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i + len - 1 <= n; i++) {
			int j = i + len - 1;
			dp[i][j] = max(dp[i + 1][j] + a[i] * (n - len + 1), dp[i][j - 1] + a[j] * (n - len + 1));
		}
	}
	cout << dp[1][n] << endl;
	return 0;
}
