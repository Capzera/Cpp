#include <bits/stdc++.h>
using namespace std;
const int N = 310;
long long n, m, ans, a[N], sum[N], dp[N][N][2];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int K = 1; K <= n; K++) {
		memset(dp, 0, sizeof(dp));
		for (int i = 1; i <= n; i++) {
			dp[i][i][0] = dp[i][i][1] = m - abs(a[i]) * K;
			ans = max(ans, dp[i][i][0]);
		}
		for (int len = 2; len <= K; len++) {
			for (int i = 1; i + len - 1 <= n; i++) {
				int j = i + len - 1, cnt = K - len + 1;
				dp[i][j][0] = max(dp[i + 1][j][0] - (a[i + 1] - a[i]) * cnt, dp[i + 1][j][1] - (a[j] - a[i]) * cnt) + m;
				dp[i][j][1] = max(dp[i][j - 1][1] - (a[j] - a[j - 1]) * cnt, dp[i][j - 1][0] - (a[j] - a[i]) * cnt) + m;
				ans = max({ans, dp[i][j][0], dp[i][j][1]});
			}
		}
	}
	cout << ans;
	return 0;
}


dp[i][j][0/1] [i..j] 落在点上剩下的元素的剩余电量最大值

dp[i][j][0] 关掉[i..j] 且人踩在a[i] 上剩下亮着的灯能量的最小值
dp[i][j][1] 关掉[i..j] 且人踩在a[j] 上剩下亮着的灯能量的最小值

dp[i][j][0] = min(dp[i + 1][j][0] + (sum[n] - (sum[j] - sum[i])) * (a[i + 1] - a[i]),   dp[i + 1][j][1])
dp[i][j][1] = min(dp[i][j - 1][1])
sum[n] - sum[j - 1] + sum[i - 1]
5 3
2 10
3 20
5 20 0
6 30
8 10


dp[i][j][0] = min(dp[i + 1][j][0] + (sum[n] - (sum[j] - sum[i])) * (a[i + 1].x - a[i].x),   dp[i + 1][j][1])
