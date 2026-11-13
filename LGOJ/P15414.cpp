#include <bits/stdc++.h>
using namespace std;
int t, n, st[2000001];
vector<int> prime;
void sieve(int n) {
	for (int i = 2; i <= n; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
}
void solve() {
	cin >> n;
	n <<= 1;
	for (auto& x : prime) {
		if (st[n - x]) continue;
		cout << x << " " << n - x << "\n";
		return;
	}
}
int main() {
	sieve(2000000);
	cin >> t;
	while (t--) solve();
	while (t--) solve();
	return 0;
}
