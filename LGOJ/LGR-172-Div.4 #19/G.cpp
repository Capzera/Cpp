#include <bits/stdc++.h>
using namespace std;
long long n, q, k, ans;
int main() {
	cin >> n >> q;
	while (q--) {
		cin >> k; k--;
		if (n >> k & 1) continue;
		long long x = n % (1ll << k);
		ans += (1ll << k) - x;
		n += (1ll << k) - x;
	}
	cout << ans;
	return 0;
}
