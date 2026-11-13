#include <bits/stdc++.h>
using namespace std;
long long k, m, n, a[201], dp[201][51];
int main() {
	cin >> k >> m >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		dp[i][0] = INT_MAX;
		for (int j = 1; j <= k; j++) {
			dp[i][j] = min(dp[i - 1][j] + a[i] - a[i - 1], dp[i - 1][j - 1] + 1);
		}
	}
	cout << dp[n][k];
	return 0;
}
AB 顺0
A  顺1
AA 顺2
B  顺3
C  翻1顺0
AC 翻1顺1
D  翻1顺2
BC 翻1顺3	
Poland cannot into space!!!
