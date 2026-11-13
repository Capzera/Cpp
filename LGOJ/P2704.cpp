#include <bits/stdc++.h>
using namespace std;
const int N = 105, M = 10;
int n, m, ans, mp[N], cnt[1 << 10];
int dp[2][1 << M][1 << M];
char ch;
vector<int> st;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) for (int j = 0; j < m; j++) {
		cin >> ch;
		if (ch == 'P') mp[i] |= 1 << j;
	}
	for (int i = 0; i < 1 << m; i++) {
		if ((i & (i << 1)) || (i & (i << 2))) continue;
		st.push_back(i);
		cnt[i] = __builtin_popcount(i);
	}
	for (int i = 1; i <= n; i++) {
		memset(dp[i & 1], -1, sizeof dp[i & 1]);
		for (int j = 0; j < st.size(); j++) {
			int s1 = st[j];
			if (s1 & ~mp[i]) continue;
			for (int k = 0; k < st.size(); k++) {
				int s2 = st[k];
				if (i > 1 && (s2 & ~mp[i - 1])) continue;
				if (s1 & s2) continue;
				for (int l = 0; l < st.size(); l++) {
					int s3 = st[l];
					if (i > 2 && (s3 & ~mp[i - 2])) continue;
					if ((s1 & s3) || (s2 & s3)) continue;
					int cur = i & 1, lst = (i - 1) & 1;
					if (dp[lst][k][l] == -1) continue;
					dp[cur][j][k] = max(dp[cur][j][k], dp[lst][k][l] + cnt[s1]);
				}
			}
		}
	}
	for (int j = 0; j < st.size(); j++) for (int k = 0; k < st.size(); k++)
		ans = max(ans, dp[n & 1][j][k]);
	cout << ans;
	return 0;
}
