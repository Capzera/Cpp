#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10, mod = 998244353;
long long t, x, y, dp[N];
long long qpow(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % mod;
		a = a * a % mod;
		b >>= 1;
	}
	return ans;
}
void solve() {
	scanf("%lld%lld", &x, &y);
	if (x < y) {
		cout << 1 << '\n';
		return;
	}
	cout << (y + 1) * qpow(2, x - y - 1) % mod << '\n';
}

int main() {
	scanf("%lld", &t);
	while (t--) solve();
	return 0;
}
