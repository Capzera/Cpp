#include <bits/stdc++.h>
using namespace std;
long long qpow(long long a, long long b, long long p) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		b >>= 1;
		a = a * a % p;
	}
	return ans;
}
long long t, a, n, p;
long long solve() {
	cin >> a >> n >> p;
	for (int i = 1; i <= n; i++) {
		a = qpow(a, i, p);
	}
	return a;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}
