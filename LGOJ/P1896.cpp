#include <bits/stdc++.h>
using namespace std;
const int N = 11;
int n, K;
long long ans, dp[N][1 << 9][N * N];
vector<int> st;
int cnt[1 << 9];
int main() {
	cin >> n >> K;
	for (int i = 0; i < 1 << n; i++) {
		if (i & (i << 1)) continue;
		st.push_back(i);
		cnt[i] = __builtin_popcount(i);
	}
	for (auto& j : st) dp[1][j][cnt[j]] = 1;
	for (int i = 2; i <= n; i++) { // 第i行
		for (auto& j : st) { // 当前状态为j
			for (auto& k : st) { // 上一状态为 k 
				if ((j & k) || (j & (k << 1)) || (j & (k >> 1))) continue; // 国王会被吃
				for (int c = cnt[j]; c <= K; c++) { // 枚举放的国王数为c
					dp[i][j][c] += dp[i - 1][k][c - cnt[j]];
				}
			}
		}
	}
	for (auto& j : st) ans += dp[n][j][K];
	cout << ans;
	return 0;
}
