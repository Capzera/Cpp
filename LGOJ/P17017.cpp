#include <bits/stdc++.h>
using namespace std;
const int p = 1e9 + 7;
long long qpow(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
long long n, m, ans = 1, inv = 1;
long long C(long long n, long long m) {
	if (m > n) return 0ll;
	for (int i = n - m + 1; i <= n; i++) {
		ans = ans * i % p;
		inv = inv * (n - i + 1) % p;
	}
	inv = qpow(inv, p - 2);
	return ans * inv % p;
}
int main() {
	cin >> m >> n;
	cout << C(n - 1, m - 1);
	return 0;
}
