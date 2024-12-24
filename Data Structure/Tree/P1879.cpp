#include <bits/stdc++.h>
using namespace std;
long long n, m, t, x, ans, a[13], dp[13][1 << 12], mod = 1e8;
vector<int> st;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = m - 1; j >= 0; j--)
			cin >> x, a[i] |= x << j;
	for (int i = 0; i < 1 << m; i++) {
		if (i & i << 1) continue;
		st.push_back(i);
	}
	for (auto& j : st) {
		if ((j | a[1]) != a[1]) continue;
		dp[1][j] = 1;
	}
	for (int i = 2; i <= n; i++) {
		for (auto& j : st) {
			if ((j | a[i]) != a[i]) continue;
			for (auto& k : st) {
				if ((k | a[i - 1]) != a[i - 1]) continue;
				if (j & k) continue;
				dp[i][j] = (dp[i][j] + dp[i - 1][k]) % mod;
			}
		}
	}
	
	for (auto& j : st) {
		ans = (ans + dp[n][j]) % mod;
	}
	cout << ans;
	return 0;
}
/*
1 2 4 5
001
010
100
101
0 0 0
0 0 1
0 1 0
0 1 1
1 0 0
1 0 1
1 1 0
1 1 1


dp[i][j] 前i行状态为j时的方案数



dp[i][j]
*/
