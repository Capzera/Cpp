#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
int t, n, ans;

void solve() {
	cin >> n;
	if (n == 6) {puts("7\n1 2 4 6 5 3"); return; }
	if (n & 1) {
		cout << n << '\n' << "2 ";
		for (int i = 4; i <= n - 2; i++) cout << i << " ";
		cout << "1 3 " << n - 1 << " " << n << '\n';
	} else {
		int k = __lg(n) + 1;
		ans = (1 << k) - 1;
		cout << ans << '\n' << "2 ";
		int a = ans >> 1;
		for (int i = 4; i <= n - 1; i++) {
			if (i == a || i == a - 1) continue;
			cout << i << " ";
		}
		cout << "1 3 " << a - 1 << " " << a << " " << n << '\n';
	}
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
