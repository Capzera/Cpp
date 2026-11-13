#include <bits/stdc++.h>
#define int long long
using namespace std;
const int p = 1e9 + 9, inv2 = 500000005, inv6 = 833333341, N = 5e6 + 5;
vector<int> prime;
bool st[N];
int phi[N], g[N];
long long t, l, r, k, n, res;
void sieve() {
	phi[1] = 1;
	for (int i = 2; i < N; i++) {
		if (!st[i]) {
			prime.push_back(i);
			phi[i] = i - 1;
		}
		for (auto& p : prime) {
			if (i * p >= N) break;
			st[i * p] = 1;
			if (i % p == 0) phi[i * p] = phi[i] * p;
			else phi[i * p] = phi[i] * (p - 1);
		}
	}
	g[0] = 0;
	for (int i = 1; i < N; i++) g[i] = (g[i - 1] + 1ll * i * phi[i]) % p;
}
unordered_map<int, int> mp;
int calc(int n) {
	if (n < N) return g[n];
	if (mp.count(n)) return mp[n];
	long long ans = 1ll * n * (n + 1) % p * (2 * n + 1) % p * inv6 % p;
	for (long long l = 2, r; l <= n; l = r + 1) {
		r = n / (n / l);
		long long sum = (l + r) * (r - l + 1) % p * inv2 % p;
		ans = (ans - sum * calc(n / l) % p + p) % p;
	}
	return mp[n] = ans;
}
long long F(long long x) {
	if (!x) return 0ll;
	return (calc(x) + 1ll) * inv2 % p;
}
void solve() {
	cin >> l >> r >> k;
	long long ans = ((F(r) - F(l - 1)) % p + p) % p * k % p;
	res = (res + ans) % p;
}
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	sieve();
	cin >> t;
	while (t--) solve();
	cout << res;
	return 0;
}
