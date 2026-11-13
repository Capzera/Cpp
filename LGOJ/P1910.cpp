#include <bits/stdc++.h>
using namespace std;
const int N = 110, M = 1e3 + 10;
int n, m1, m2, w1[N], w2[N], c[N], dp[M][M];
int main() {
	cin >> n >> m1 >> m2;
	for (int i = 1; i <= n; i++) {
		cin >> c[i] >> w1[i] >> w2[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = m1; j >= w1[i]; j--) {
			for (int k = m2; k >= w2[i]; k--) {
				dp[j][k] = max(dp[j][k], dp[j - w1[i]][k - w2[i]] + c[i]);
			}
		}
	}
	cout << dp[m1][m2];
	return 0;
}
