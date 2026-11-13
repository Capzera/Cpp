#include <bits/stdc++.h>
using namespace std;
long long l, r, m, ans;
bool st[1000001], vis[46350];
int main() {
	cin >> l >> r;
	l += l == 1;
	m = sqrt(r);
	/*
	for (int i = 2; i <= m; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && 1ll * i * prime[j] <= m; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	memset(st, 0, sizeof(st));
	for (auto& p : prime) {
		for (long long j = 1ll * l / p * p; j <= r; j += p) {
			if (j < l || j == p || st[j - l]) continue;
			ans++, st[j - l] = 1;
		}
	}
	*/
	for (int i = 2; i <= m; i++) {
		if (vis[i]) continue;
		for (int j = i * i; j <= m; j += i) vis[j] = 1;
		for (long long j = (l + i - 1) / i * i; j <= r; j += i) {
			if (st[j - l]) continue;
			ans++, st[j - l] = 1;
		}
	}
	cout << r - l + 1 - ans;
	return 0;
}
