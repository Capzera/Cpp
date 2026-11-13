#include <bits/stdc++.h>
using namespace std;
long long t, a, p; 
long long qpow(long long a, int b, int p) {
	long long res = 1;
	while (b) {
		if (b & 1) res = res * a % p;
		b >>= 1;
		a = a * a % p;
	}
	return res;
}
bool solve() {
	cin >> a >> p;
	p--;
	for (int i = 2; i * i <= p; i++) {
		if (p % i) continue;
		if (qpow(a, i, p + 1) == 1 || qpow(a, p / i, p + 1) == 1) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
