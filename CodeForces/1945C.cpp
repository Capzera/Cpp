#include <bits/stdc++.h>
using namespace std;
void solve() {
	int n, ans = -1, sum = 0, total = 0;
	string s;
	cin >> n >> s;
	for (int i = 0; i < n; i++) {
		total += s[i] - '0';
	}
	for (int i = 0; i <= n; i++) {
		if (i - sum >= (i + 1) / 2 && total - sum >= (n - i + 1) / 2) {
			if (abs(n / 2.0 - i) < abs(n / 2.0 - ans)) ans = i;
		}
		if (i < n) sum += s[i] - '0';
	}
	cout << ans << endl;
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
/*

i - left > (i + 1) / 2
sum - left > (n - i + 1) / 2
*/
