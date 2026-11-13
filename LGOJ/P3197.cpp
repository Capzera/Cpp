#include <bits/stdc++.h>
using namespace std;
const int p = 1e5 + 3;
long long m, n;
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
	cin >> m >> n;
	cout << (qpow(m, n) - m * qpow(m - 1, n - 1) % p + p) % p;
	return 0;
}
