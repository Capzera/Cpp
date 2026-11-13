#include <bits/stdc++.h>
using namespace std;
string l, r;
void solve() {
	cin >> l >> r;
	int n = l.size();
	vector<vector<vector<int> > > dp(n + 1, vector<vector<int> >(2, vector<int>(2, 1e9)));
	dp[0][1][1] = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				if (dp[i][j][k] >= 1e9) continue;
				int lo = (j == 1 ? l[i] - '0' : 0), hi = (k == 1 ? r[i] - '0' : 9);
				for (int x = lo; x <= hi; x++) {
					int nj = (j == 1 ? x == l[i] - '0' : 0);
					int nk = (k == 1 ? x == r[i] - '0' : 0);
					int add = 0;
					if (l[i] == r[i]) {
						if (x == l[i] - '0') add = 2;
					} else {
						if (x == l[i] - '0' || x == r[i] - '0') add = 1;
					}
					dp[i + 1][nj][nk] = min(dp[i + 1][nj][nk], dp[i][j][k] + add);
				}
			}
		}
	}
	int ans = 1e9;
	for (int j = 0; j < 2; j++)
		for (int k = 0; k < 2; k++)
			ans = min(ans, dp[n][j][k]);
	cout << ans << endl;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) solve();
	return 0;
}

