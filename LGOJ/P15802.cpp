#include <iostream>
using namespace std;
const int mod = 1e9;
long long qpow(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % mod;
		b >>= 1;
		a = a * a % mod;
	}
	return ans;
}
int t, n;
long long solve() {
	cin >> n;
	if (n == 1) return 1;
	if (n % 3 == 0) return qpow(3, n / 3);
	if (n % 3 == 1) return 4ll * qpow(3, n / 3 - 1) % mod;
	return qpow(3, n / 3) * 2 % mod;
}

int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
