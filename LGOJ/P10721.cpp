#include <bits/stdc++.h>
using namespace std;
int n, m, cnt, a[21], dp[34000];
long long ans;
string s;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cin >> m >> s;
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= m / 3; j++) {
			dp[j] = max(dp[j], dp[j - i] + a[i]);
		}
	}
	for (int i = 0; i < m - 2; i++) {
		if (s.substr(i, 3) == "abc") {
			cnt++, i += 2;
			continue;
		} else {
			ans += dp[cnt];
			cnt = 0;
		}
	}
	if (cnt) ans += dp[cnt];
	cout << ans;
	return 0;
}

