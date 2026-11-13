#include <bits/stdc++.h>
using namespace std;
const int N = 105, M = 2e4 + 5;
long long n, a[N], b[N], dp[M][2];
void solve() {
	cin >> n;
	long long sum = 0, S = 0, ans = LLONG_MAX;
	memset(dp, 0, sizeof dp);
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = 1; i <= n; i++) {
		sum += a[i] * a[i] + b[i] * b[i];
		S += a[i] + b[i];
	}
	dp[0][0] = 1;
	for (int k = 1; k <= n; k++) {
		int j = k & 1, lst = j ^ 1;
		for (int i = 0; i <= S; i++) dp[i][j] = 0;
		for (int i = 0; i <= S; i++) {
			if (!dp[i][lst]) continue;
			dp[i + a[k]][j] = dp[i + b[k]][j] = 1;
		}
	}
	for (int X = 0; X <= S; X++) {
		if (!dp[X][n & 1]) continue;
		ans = min(ans, 1ll * (n - 2) * sum + X * X + (S - X) * (S - X));
	}
	cout << ans << '\n';
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}
//https://codeforces.com/problemset/problem/1637/D
/*


*/
