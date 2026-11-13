#include <bits/stdc++.h>
using namespace std;
void solve() {
	long long x, y, a, b, ans;
	cin >> x >> y >> a >> b;
	ans = min(a * (x + y), b * (min(x, y)) + a * (abs(x - y)));
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
