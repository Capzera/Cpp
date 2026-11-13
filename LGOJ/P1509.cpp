#include <bits/stdc++.h>
using namespace std;
const int N = 110;
int n, m1, m2, w1[N], w2[N], c[N], dp[N][N][2];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> w1[i] >> w2[i] >> c[i];
	}
	cin >> m1 >> m2;
	for (int i = 1; i <= n; i++) {
		for (int j = m1; j >= w1[i]; j--) {
			for (int k = m2; k >= w2[i]; k--) {
				if (dp[j][k][0] < dp[j - w1[i]][k - w2[i]][0] + 1) {
					dp[j][k][0] = dp[j - w1[i]][k - w2[i]][0] + 1;
					dp[j][k][1] = dp[j - w1[i]][k - w2[i]][1] + c[i];
				} else if (dp[j][k][0] == dp[j - w1[i]][k - w2[i]][0] + 1) {
					dp[j][k][1] = min(dp[j][k][1], dp[j - w1[i]][k - w2[i]][1] + c[i]);
				}
			}
		}
	}
	cout << dp[m1][m2][1];
	return 0;
}
/*

dp[j][k][s=0] 前i个物品，质量1为j，质量2为k，能够买物品数量的最大值
dp[j][k][s=1] 前i个物品，质量1为j，质量2为k，花费的最小值
*/
