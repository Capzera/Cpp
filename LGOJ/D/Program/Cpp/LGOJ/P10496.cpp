#include <bits/stdc++.h>
#define int long long
using namespace std;
long long phi(long long n) {
	long long ans = n, m = sqrt(n);
	for (int i = 2; i <= m; i++) {
		if (n % i) continue;
		while (n % i == 0) n /= i;
		ans -= ans / i;
	}
	if (n > 1) ans -= ans / n;
	return ans;
}
long long qpow(__int128 a, int b, int p) {
	__int128 ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		b >>= 1;
		a = a * a % p;
	}
	return ans;
}
long long t, k, num;
int solve() {
	if (__gcd(10ll, k) != 1) return 0;
	int i = 1, ph = phi(k);
	vector<int> fac;
	for (; i * i < ph; i++) {
		if (ph % i) continue;
		fac.push_back(i);
	}
	if (i * i == ph) fac.push_back(i);
	for (int i = fac.size() - 1; ~i; i--) {
		fac.push_back(ph / fac[i]);
	}
	for (auto& x : fac) {
		if (qpow(10ll, x, k) == 1) return x;
	}
	return 1;
}

signed main() {
	while (cin >> k, k) {
		k = 9 * k / __gcd(k, 8ll);
		printf("Case %d: ", ++num);
		printf("%lld\n", solve());
	}
	return 0;
}
