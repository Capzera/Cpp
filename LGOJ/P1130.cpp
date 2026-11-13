#include <bits/stdc++.h>
using namespace std;
long long n, m, ans = INT_MAX;
int main() {
	cin >> n >> m;
	vector<vector<long long> > dp(m, vector<long long> (n, INT_MAX));
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cin >> dp[i][j];
		}
	}
	for (int j = n - 2; j >= 0; j--) {
		for (int i = 0; i < m; i++) {
			dp[i][j] += min(dp[i][j + 1], dp[(i + 1) % m][j + 1]);
		}
	}
	for (int i = 0; i < m; i++) ans = min(ans, dp[i][0]);
	cout << ans;
	return 0;
}
