#include <bits/stdc++.h>
using namespace std;
int t, n, m, x, p;
int solve() {
	scanf("%d%d", &n, &m);
	unordered_map<int ,int> mp;
	int g = 0, ans = 0;
	for (int i = 1; i <= n; i++) {
		scanf("%d", &x);
		mp[x]++;
	}
	for (auto&& [_, x] : mp) g = __gcd(g, x);
	for (int d = 1; d <= m; d++) {
		if (g % (n / __gcd(d, n))) continue;
		ans++;
	}
	return ans;
}

int main() {
	scanf("%d", &t);
	while (t--) printf("%d\n", solve());
	return 0;
}
