#include <bits/stdc++.h>
using namespace std;
long long n, ans, a[200001], l[200001], r[200001], sum[200001], mod = 16777216;
int lowbit(int x) {
	return x & (-x);
}
void push(int x, int a) {
	while (x <= n) {
		sum[x] = (sum[x] + a + mod) % mod;
		x += lowbit(x);
	}
}

long long get(int x) {
	long long res = 0;
	while (x) {
		res = (res + sum[x] + mod) % mod;
		x -= lowbit(x);
	}
	return res;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		l[i] = get(a[i]);
		r[i] = a[i] - l[i] - 1;
		push(a[i], 1);
	}
	memset(sum, 0, sizeof(sum));
	for (int i = 1; i <= n; i++) {
		ans = (ans + (1ll * (n - i - r[i]) * (i - 1) % mod * l[i] % mod) + mod) % mod;
		ans = (ans - (1ll * get(a[i]) * (n - i - r[i]) % mod) + mod) % mod;
		ans = (ans - (1ll * (l[i] - 1) * l[i] / 2 % mod * (n - i - r[i]) % mod) + mod) % mod;
		push(a[i], i);
	}
	memset(sum, 0, sizeof(sum));
	for (int i = n; i >= 1; i--) {
		ans = (ans + (1ll * l[i] * (get(n) - get(a[i])) % mod) + mod) % mod;
		push(a[i], n - i - r[i]);
	}
	for (int i = 1; i <= n; i++) {
		long long t = n - i - r[i];
		if (t >= 3) {
			ans = (ans - 1ll * t * (t - 1) / 2 * (t - 2) / 3 % mod + mod) % mod;
		}
	}
	memset(sum, 0, sizeof(sum));
	for (int i = 1; i <= n; i++) {
		ans = (ans + 1ll * l[i] * r[i] % mod * (n - i - r[i]) % mod + mod) % mod;
		ans = (ans + 1ll * l[i] * (l[i] - 1) / 2 % mod * (n - i - r[i]) % mod + mod) % mod;
		ans = (ans - (1ll * get(a[i]) * (n - i - r[i]) % mod) + mod) % mod;
		push(a[i], l[i] + r[i]);
	}
	cout << ans;
	return 0;
}
