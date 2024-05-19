#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, K;
	cin >> n >> K;
	K = min(n, K);
	vector<vector<long long> > a(n + 1, vector<long long>(n + 1));
	vector<vector<vector<long long> > > dp(n + 1, vector<vector<long long> > (n + 1, vector<long long> (K + 1, -3e9)));
		for (int j = 0; j <= n; j++)
			for (int k = 0; k <= K; k++) 
				dp[0][j][k] = 0;
	long long ans = -3e9;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			cin >> a[i][j];
			dp[i][j][0] = max(dp[i - 1][j][0], dp[i - 1][j - 1][0]) + a[i][j];
			for (int k = 1; k <= K; k++) {
				dp[i][j][k] = max(dp[i - 1][j][k], dp[i - 1][j - 1][k]) + a[i][j];
				dp[i][j][k] = max(dp[i][j][k], max(dp[i - 1][j][k - 1], dp[i - 1][j - 1][k - 1]) + 3ll * a[i][j]);
			}
		}
	}
	for (int j = 1; j <= n; j++)
		for (int k = 0; k <= K; k++)
			ans = max(ans, dp[n][j][k]);	
	cout << ans << endl;
	return 0;
}
