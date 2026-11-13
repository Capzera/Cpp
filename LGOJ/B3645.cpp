#include <bits/stdc++.h>
using namespace std;
const int p = 1145141;
long long n, m, l, r, ans, a[1000010], pro[1000010], inv[1000010];
long long ksm(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
int main() {
	cin >> n >> m;
	pro[0] = inv[0] = 1;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		pro[i] = pro[i - 1] * a[i] % p;
	}
	inv[n] = ksm(pro[n], p - 2);
	for (int i = n - 1; i > 0; i--) inv[i] = inv[i + 1] * a[i + 1] % p;
	while (m--) {
		cin >> l >> r;
		ans ^= pro[r] * inv[l - 1] % p;
	}
	cout << ans;
	return 0;
}
