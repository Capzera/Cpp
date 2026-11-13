#include <bits/stdc++.h>
using namespace std;
const int N = 510, M = 45010;
int n, m, w, c, dp[M];
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> w;
		for (int j = m; j >= w; j--) {
			dp[j] = max(dp[j], dp[j - w] + w);
		}
	}
	cout << dp[m];
	return 0;
}
