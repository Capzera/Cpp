#include <bits/stdc++.h>
using namespace std;
int t, m, k, x;
int phi(int n) {
	int ans = n, m = sqrt(n);
	for (int i = 2; i <= m; i++) {
		if (n % i) continue;
		while (n % i == 0) n /= i;
		ans -= ans / i;
	}
	if (n > 1) ans -= ans / n;
	return ans;
}
void solve() {
	cin >> m >> k;
	int c1 = 0, c2 = 0, len = 0;
	unordered_set<int> num = {9, 8, 7, 6, 5, 4, 3, 2, 1};
	while (k--) {
		scanf("%d", &x);
		num.erase(x);
	}
	if (num.empty()) {
		puts("-1");
		return;
	}
	while (m % 2 == 0) m /= 2, c1++;
	while (m % 5 == 0) m /= 5, c2++;
	len = c1 || c2;
	m *= 9;
	int t = *num.begin();
	k = m / __gcd(t, m);
	int p = phi(k);
	printf("%d\n%d %d\n", len + 1, t, p);
	if (len) printf("0 %d\n", max(c1, c2));
}

int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
