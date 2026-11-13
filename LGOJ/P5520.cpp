#include <bits/stdc++.h>
using namespace std;
long long type, n, m, p, ans = 1;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> type >> n >> m >> p;
	if (m > n) return puts("0"), 0;
	for (int i = n - 2 * m + 2; i <= n - m + 1; i++) {
		ans = ans * i % p;
	}
	cout << ans;
	return 0;
}

((k + 1)^n - (k + 1)) * qpow(k, p - 2) % p;

cout <<  (k + 1) * (qpow(k + 1, n) - 1) % p * qpow(k, p - 2) % p << endl;
