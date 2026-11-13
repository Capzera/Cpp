#include <bits/stdc++.h>
using namespace std;
const int N = 102;
long long n, a[N], dp[N][N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int len = 1; len <= n; len++) {
		for (int i = 1; i + len - 1 <= n; i++) {
			int j = i + len - 1;
			for (int k = i; k <= j; k++) {
				dp[i][j] = max(dp[i][j], dp[i][k - 1] + dp[k + 1][j] + a[i - 1] + a[j + 1]);
			}
		}
	}
	cout << dp[1][n];
	return 0;
}


