#include <bits/stdc++.h>
using namespace std;
const int M = 25010, N = 110;
int t, n, a[N];
bool dp[M];
void solve() {
	cin >> n;
	int ans = n;
	memset(dp, 0, sizeof(dp)); dp[0] = 1;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (dp[a[i]]) {
			ans--;
			continue;
		}
		for (int j = a[i]; j <= a[n]; j++) {
			dp[j] = dp[j] | dp[j - a[i]];
		}
	}
	cout << ans << '\n';
}

int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
