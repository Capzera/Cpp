#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
struct point {
	long long x, y, v;
	bool operator < (const struct point& rhs) const {
		if (x != rhs.x) return x < rhs.x;
		return y < rhs.y;
	}
} a[N];
int n;
long long x0, sumy, sumv, sum[N], dp[N][N][2];
int main() {
	memset(dp, 0x3f, sizeof(dp));
	cin >> n >> x0;
	for (int i = 1; i <= n; i++) cin >> a[i].x;
	for (int i = 1; i <= n; i++) cin >> a[i].y, sumy += a[i].y;
	for (int i = 1; i <= n; i++) cin >> a[i].v, sumv += a[i].v;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		sum[i] = sum[i - 1] + a[i].v;
		dp[i][i][0] = dp[i][i][1] = abs(a[i].x - x0) * sumv;
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i + len - 1 <= n; i++) {
			int j = i + len - 1;
			long long cost0 = sum[n] - (sum[j] - sum[i]);
			long long cost1 = sum[n] - (sum[j - 1] - sum[i - 1]);
			dp[i][j][0] = min(dp[i + 1][j][0] + (a[i + 1].x - a[i].x) * cost0, dp[i + 1][j][1] + (a[j].x - a[i].x) * cost0);
			dp[i][j][1] = min(dp[i][j - 1][1] + (a[j].x - a[j - 1].x) * cost1, dp[i][j - 1][0] + (a[j].x - a[i].x) * cost1);
		}
	}
	printf("%.3lf", (sumy - min(dp[1][n][0], dp[1][n][1])) / 1000.0);
	return 0;
}
