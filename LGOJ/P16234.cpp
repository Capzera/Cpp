#include <bits/stdc++.h>
using namespace std;
long long t, n, x, y;
void solve() {
	cin >> n >> x >> y;
	cout << max(0ll, y - x + 1) << '\n';
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
