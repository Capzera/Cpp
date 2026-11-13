#include <bits/stdc++.h>
using namespace std;

long long phi(long long n) {
	int ans = n, m = sqrt(n);
	for (int i = 2; i <= m; i++) {
		if (n % i) continue;
		while (n % i == 0) n /= i;
		ans -= ans / i;
	}
	if (n > 1) ans -= ans / n;
	return ans;
}
long long qpow(long long a, long long b, long long p) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
long long t, a, n, p, f, mod;
long long fac(long long x) {
	if (!x) return 1;
	long long ans = x * fac(x - 1);
	if (ans >= mod) f = 1;
	return ans % mod;
}
long long solve() {
	cin >> a >> n >> p;
	f = 0, mod = phi(p);
	long long b = fac(n) + f * mod;
	return qpow(a, b, p);
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}
