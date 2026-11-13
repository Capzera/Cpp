#include <bits/stdc++.h>
using namespace std;
long long T, n, d, e, p, q;
void solve() {
	cin >> n >> d >> e;
	long long b = (n - e * d + 2);
	long long delta = b * b - 4 * n;
	long long sq = sqrt(delta);
	if (delta < 0 || sq * sq != delta) {
		puts("NO");
		return;
	}
	p = (b - sq) / 2, q = (b + sq) / 2;
	if (p > q) swap(p, q);
	if (p > 0 && q > 0 && p * q == n) {
		cout << p << " " << q << '\n';
		return;
	}
	puts("NO");
}
int main() {
	cin >> T;
	while (T--) solve();
	return 0;
}

