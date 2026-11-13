#include <bits/stdc++.h>
using namespace std;
const int M = 210, N = 110;
int n, m1, m2, w1[N], w2[N], c[N], dp[M][M];
string ans[M][M];
int main() {
	cin >> m1 >> m2 >> n;
	for (int i = 1; i <= n; i++) {
		cin >> w1[i] >> w2[i] >> c[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = m1; j >= w1[i]; j--) {
			for (int k = m2; k >= w2[i]; k--) {
				string s = ans[j - w1[i]][k - w2[i]] + char(i);
				if (dp[j - w1[i]][k - w2[i]] + c[i] > dp[j][k]) {
					dp[j][k] = dp[j - w1[i]][k - w2[i]] + c[i];
					ans[j][k] = s;
				} else if (dp[j - w1[i]][k - w2[i]] + c[i] == dp[j][k]) {
					ans[j][k] = min(ans[j][k], s);
				}
			}
		}
	}
	cout << dp[m1][m2] << '\n';
	for (auto& x : ans[m1][m2]) {
		cout << int(x) << " ";
	}
	return 0;
}
