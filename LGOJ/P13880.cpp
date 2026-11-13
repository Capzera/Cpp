#include <bits/stdc++.h>
using namespace std;
const int p = 998244353;
long long n, ans, a, b;

int phi(long long n) {
	long long ans = n, m = sqrt(n);
	for (int i = 2; i <= m; i++) {
		if (n % i) continue;
		while (n % i == 0) n /= i;
		ans -= ans / i;
	}
	if (n > 1) ans -= ans / n;
	return ans;
}

long long qpow(long long a, long long b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		b >>= 1;
		a = a * a % p;
	}
	return ans;
}

int main() {
	cin >> a >> b;
	ans = phi(a) * qpow(a, b - 1) % p;
	cout << ans;
	return 0;
}
