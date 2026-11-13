#include <bits/stdc++.h>
using namespace std;
const long long p = 998244353, N = 5001;
long long n, ans, V, a[N], dp[N], f[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i], V = max(V, a[i]);
	sort(a + 1, a + n + 1);
	dp[0] = dp[a[1]] = 1;
	for (int i = 2, base = 4; i < n; i++, base = base * 2 % p) {
		for (int j = V; j >= a[i]; j--) {
			dp[j] = (dp[j] + dp[j - a[i]]) % p;
		}
		for (int j = 0; j <= a[i + 1]; j++) {
			f[i] = (f[i] + dp[j]) % p;
		}
		ans = (ans + (base - f[i] + p)) % p;
	}
	cout << ans;
	return 0;
}
