#include <bits/stdc++.h>
using namespace std;
const int N = 2e3 + 5, mod = 998244353;
int n, q, l, r;
long long a[N];
long long solve() {
	cin >> l >> r;
	int cnt1 = 0, cnt0 = 0;
	long long ans = 1;
	vector<long long> od, ev;
	for (int i = l; i <= r; i++) {
		if (a[i] == 0) {
			cnt0++;
			continue;
		}
		if (a[i] == 1) {
			cnt1++;
			continue;
		}
		a[i] & 1 ? od.push_back(a[i]) : ev.push_back(a[i]);
	}
	if (cnt1 + cnt0 == r - l + 1) return cnt1 > 0;
	sort(ev.begin(), ev.end());
	for (auto& x : ev) {
		if (cnt1) {
			ans = ans * ((x + 1) % mod) % mod;
			cnt1--;
		} else ans = ans * x % mod;
	}
	for (auto& x : od) ans = ans * x % mod;
	return ans;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> q;
	for (int i = 1; i <= n; i++) cin >> a[i];
	while (q--) cout << solve() << '\n';
	return 0;
}
