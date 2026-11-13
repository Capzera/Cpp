#include <bits/stdc++.h>
using namespace std;
long long n, s, c[5], d[5], dp[10001];
int main() {
	for (int i = 1; i <= 4; i++) cin >> c[i];
	cin >> n;
	dp[0] = 1;
	for (int i = 1; i <= 4; i++) {
		for (int j = c[i]; j <= 10000; j++) {
			dp[j] += dp[j - c[i]];
		}
	}
	while (n--) {
		for (int i = 1; i <= 4; i++) cin >> d[i];
		cin >> s;
		long long ans = 0;
		for (int i = 0; i <= 15; i++) {
			long long rest = s, cnt = 0;
			for (int j = 1; j <= 4; j++) {
				if (i >> j - 1 & 1) rest -= c[j] * (d[j] + 1), cnt ^= 1;
			}
			if (rest < 0) continue;
			if (!cnt) ans += dp[rest];
			else ans -= dp[rest];

		}
		cout << ans << '\n';
	}
	return 0;
}
