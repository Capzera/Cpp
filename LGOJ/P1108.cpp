#include <bits/stdc++.h>
using namespace std;
int n, mxlen, ans;
int main() {
	cin >> n;
	vector<int> a(n), dp(n), cnt(n);
	for (auto& x : a) cin >> x;
	for (int i = 0; i < n; i++) {
		dp[i] = 1;
		for (int j = 0; j < i; j++) {
			if (a[j] <= a[i]) continue;
			dp[i] = max(dp[i], dp[j] + 1);
		}
		mxlen = max(mxlen, dp[i]);
		if (dp[i] == 1) cnt[i] = 1;
		for (int j = 0; j < i; j++) {
			if (dp[j] + 1 == dp[i] && a[j] > a[i]) {
				cnt[i] += cnt[j];
			} else if (a[j] == a[i] && dp[j] == dp[i]) {
				cnt[i] = 0;
			}
		}
	}
	for (int i = 0; i < n; i++) ans += cnt[i] * (dp[i] == mxlen);
	cout << mxlen << " " << ans;
	return 0;
}
