#include <bits/stdc++.h>
using namespace std;
const long long p = 1e9 + 7;
long long n, m, f, x, k, q;
long long fac[2000010], inv[2000010];

long long qpow(long long a, int x) { // 求快速幂
	long long res = 1;
	while (x > 0) {
		if (x & 1) res = res * a % p;
		x >>= 1;
		a = a * a % p;
	}
	return res;
}

void init(int n) { // O(n + logp)
	fac[0] = 1;
	for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % p; // O(n)
	inv[n] = qpow(fac[n], p - 2); // O(log p)
	for (int i = n - 1; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1) % p; // O(n)
}

long long C(int n, int m) {
	if (n < m) return 0LL;
	return (fac[n] * inv[m]) % p * inv[n - m] % p;
}
int main() {
	cin >> n >> m >> k >> q;
	q %= p;
	init(n);
	for (long long i = 0, j = 1; i <= m; i++, j = j * q % p) {
		cin >> x;
		f = (f + x * j) % p;
	}
	cout << C(n, k) * f % p;
	return 0;
}
