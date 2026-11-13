#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int mod = 1000000007;
const int R = 31;
long long g[R + 2]; 

void init() {
	g[0] = 1;
	for (int i = 1; i <= R; i++) {
		g[i] = (1LL * g[i - 1] * g[i - 1]) % mod;
		g[i] = (1LL * g[i] * i) % mod;
	}
}

long long dfs(int r, long long k) {
	if (r > R) 
		r = R;
	if (k <= 0 || r == 0)
		return 1;
	long long len = (1LL << r) - 1;
	if (k >= len) 
		return g[r];
	long long left_len = (1LL << (r - 1)) - 1;
	if (k <= left_len) {
		return dfs(r - 1, k);
	} else if (k == left_len + 1) {
		return (1LL * dfs(r - 1, left_len) * r) % mod;
	} else {
		long long left_product = dfs(r - 1, left_len);
		long long right_product = dfs(r - 1, k - left_len - 1);
		return (1LL * left_product * r % mod) * right_product % mod;
	}
}
int n;
long long k;
void solve() {
	cin >> n >> k;
	vector<long long> a(n), dis;
	long long r = 0, ans = 1;
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	for (int i = 0; i < n; i++) {
		if (a[i] == r + 1) {
			r = a[i];
		} else if (a[i] > r + 1) {
			break;
		}
	}
	for (auto& x : a) {
		if (x <= r) continue;
		dis.push_back(x);
	}
	if (r > 0) {
		if (r <= R) {
			long long cont_len = (1LL << r) - 1;
			if (cont_len <= k) {
				ans = (ans * g[r]) % mod;
				k -= cont_len;
			} else {
				ans = (ans * dfs(r, k)) % mod;
				k = 0;
			}
		} else {
			ans = (ans * dfs(r, k)) % mod;
			k = 0;
		}
	}
	for (int i = 0; i < dis.size() && k > 0; i++) {
		long long m = dis[i];
		ans = (ans * (m % mod)) % mod;
		k--;
		if (k == 0) break;
		
		long long nr = m - 1;
		if (nr > 0) {
			if (nr <= R) {
				long long new_len = (1LL << nr) - 1;
				if (new_len <= k) {
					ans = (ans * g[nr]) % mod;
					k -= new_len;
				} else {
					ans = (ans * dfs(nr, k)) % mod;
					k = 0;
				}
			} else {
				ans = (ans * dfs(nr, k)) % mod;
				k = 0;
			}
		}
	}
	cout << ans % mod << endl;
}

int main() {
	init();
	int t = 1;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
