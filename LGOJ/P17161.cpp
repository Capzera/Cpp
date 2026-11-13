#include <bits/stdc++.h>
using namespace std;
const int N = 1e7 + 3;
long long t, n, m;
int st[N];
vector<int> prime;
void init() {
	for (int i = 2; i <= 1e7; i++) {
		if (!st[i]) {
			prime.push_back(i);
			st[i] = i;
		}
		for (auto& p : prime) {
			if (p > st[i] || 1ll * i * p > 1e7) break;
			st[i * p] = p;
		}
	}
}
void get_fac(int x, vector<int>& a) {
	while (x > 1) {
		int p = st[x];
		a.push_back(p);
		while (x % p == 0) x /= p;
	}
}
long long solve() {
	scanf("%lld%lld", &n, &m);
	vector<int> fx, fy;
	get_fac(n, fx), get_fac(m, fy);
	long long ans = 1;
	int i = 0, j = 0, sx = fx.size(), sy = fy.size();
	while (i < sx && j < sy) {
		if (fx[i] == fy[j]) {
			i++, j++;
			continue;
		}
		ans *= fx[i] < fy[j] ? fx[i++] : fy[j++];
	}
	while (i < sx) ans *= fx[i++];
	while (j < sy) ans *= fy[j++];
	return ans;
}
int main() {
	init();
	cin >> t;
	while (t--) printf("%lld\n", solve());
	return 0;
}
