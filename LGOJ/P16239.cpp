#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10, mod = 998244353;
struct node {
	long long id, r, b;
	bool operator < (const struct node& rhs) const {
		return r * rhs.b < rhs.r * b;
	}
};
long long n, m, ret = 1, a[N], b[N], f[N], k[N];
bool check(long long x) {
	long long sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += max(x - f[i], 0ll);
		if (sum > m) return 0;
	}
	return sum <= m;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> b[i];
		f[i] = a[i] / b[i];
	}
	long long l = 0, r = 2e9 + 1, ans = 0;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	vector<node> rem;
	for (int i = 1; i <= n; i++) {
		k[i] = max(ans - f[i], 0ll);
		m -= k[i];
		if (f[i] > ans) continue;
		rem.push_back({i, a[i] % b[i], b[i]});
	}
	sort(rem.begin(), rem.end());
	for (int i = 0; i < m; i++) k[rem[i].id]++;
	for (int i = 1; i <= n; i++) {
		long long val = (a[i] + (k[i] * b[i]) % mod) % mod;
		ret = ret * val % mod;
	}
	cout << ret << endl;
	return 0;
}
