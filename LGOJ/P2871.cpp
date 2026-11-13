#include <bits/stdc++.h>
using namespace std;
const int N = 5010, M = 45010;
int n, m, w, c, dp[M];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> w >> c;
		for (int j = m; j >= w; j--) {
			dp[j] = max(dp[j], dp[j - w] + c);
		}
	}
	cout << dp[m];
	return 0;
}
