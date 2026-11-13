#include <bits/stdc++.h>
#if defined (_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#endif
using namespace std;
typedef long long ll;
const int N = 2e5 + 5;
const ll mod = 998244353, inv2 = 499122177, inv6 = 166374059, inv24 = 291154603;
vector<int> g[N];
ll t, n, k, sz[N], dp1[N], dp2[N], dis1[N], dis2[N]; // 换根前，换根后
ll fac[N], inv[N];
inline ll read() {
	ll ans = 0, f = 1;
	char ch = getchar_unlocked();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar_unlocked();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar_unlocked();
	}
	return ans * f;
}
inline void write(ll x) {
	if (x < 0) putchar_unlocked('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar_unlocked(x % 10 + '0');
}
ll qpow(ll a, ll b) {
	ll res = 1;
	while (b) {
		if (b & 1) res = res * a % mod;
		a = a * a % mod;
		b >>= 1;
	}
	return res;
}
void init() {
	fac[0] = 1;
	for (int i = 1; i < N; i++) fac[i] = fac[i - 1] * i % mod;
	inv[N - 1] = qpow(fac[N - 1], mod - 2);
	for (int i = N - 2; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1) % mod;
}
inline ll C(ll n, ll m) {
	if (n < m || m < 0) return 0;
	return fac[n] * inv[m] % mod * inv[n - m] % mod;
}
void dfs1(int u, int fa) {
	sz[u] = 1;
	dp1[u] = dp2[u] = 0;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs1(v, u);
		sz[u] += sz[v];
		dp1[u] = (dp1[u] + dp1[v] + sz[v]) % mod;
		dp2[u] = (dp2[u] + dp2[v] + 2 * dp1[v] + sz[v]) % mod;
	}
}
void dfs2(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dis1[v] = (dis1[u] + n - 2 * sz[v]) % mod;
		if (dis1[v] < 0) dis1[v] += mod;
		ll c = (dp1[v] + sz[v]) % mod;
		dis2[v] = (dis2[u] + 2 * dis1[u] % mod - 4 * c % mod + n) % mod;
		if (dis2[v] < 0) dis2[v] = (dis2[v] % mod + mod) % mod;
		dfs2(v, u);
	}
}
void solve() {
	n = read(), k = read();
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1, u, v; i < n; i++) {
		u = read(), v = read();
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs1(1, 0);
	dis1[1] = dp1[1], dis2[1] = dp2[1];
	dfs2(1, 0);
	ll sum1 = 0, sum2 = 0;
	for (int i = 1; i <= n; i++) {
		sum1 = (sum1 + dis1[i]) % mod;
		sum2 = (sum2 + dis2[i]) % mod;
	}
	sum1 = sum1 * inv2 % mod;
	sum2 = sum2 * inv2 % mod;
	ll w = n % mod;
	if (k == 2) w = C(n, 2);
	if (k == 3) w = (sum1 - C(n, 2) + mod) % mod;
	if (k == 4) {
		w = (sum2 - 3 * sum1 % mod + 2 * C(n, 2) % mod) % mod;
		w = (w + mod) % mod * inv2 % mod;
	}

	ll ans = (2 * C(n, k) % mod - w + mod) % mod;
	write(ans), putchar_unlocked('\n');
}
int main() {
	init();
	t = read();
	while (t--) solve();
	return 0;
}
