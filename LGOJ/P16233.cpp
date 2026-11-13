#include <bits/stdc++.h>
using namespace std;
const int mod = 998244353;
long long qpow(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % mod;
		b >>= 1;
		a = a * a % mod;
	}
	return ans;
}
int main() {
	cout << 2026 * qpow(2, 2025) % mod;
	return 0;
}
