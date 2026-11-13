#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;

ll m, n, ans, mn = LLONG_MAX;

ll phi(ll n) {
	ll ans = n;
	for (ll i = 2; i * i <= m; i++) {
		if (n % i) continue;
		while (n % i == 0) n /= i;
		ans -= ans / i;
	}
	if (n > 1) ans -= ans / n;
	return ans;
}

ll qpow(__int128 a, ll b, ll p) {
	ll ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		b >>= 1;
		a = a * a % p;
	}
	return ans;
}

signed main() {
	cin >> n;
	n *= 9;
	for (ll t = 1; t <= 9; t++) {
		m = n / gcd(n, t);
		ll p = phi(m), i = 1;
		vector<ll> fac;
		for (; i * i <= p && i <= mn; i++) {
			if (p % i) continue;
			fac.push_back(i);
		}
		for (int i = fac.size() - 1; ~i; i--) {
			if (p / fac[i] > mn) continue;
			if (p / fac[i] != fac[i]) fac.push_back(p / fac[i]);
		}
		for (auto& x : fac) {
			if (qpow(10, x, m) != 1 || x >= mn) continue;
			mn = x;
			ans = t;
		}
	}
	if (mn == LLONG_MAX) cout << -1;
	else cout << (qpow(10, mn, mod) - 1) * ans % mod * qpow(9, mod - 2, mod) % mod;
	return 0;
}
