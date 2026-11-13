#include <bits/stdc++.h>
using namespace std;
int t, n, m, k, p;
string solve() {
	cin >> n >> m >> k >> p;
	if (k > m) return "Divide";
	if (p < (m - k + 1) * n) return "Together";
	return "Chance";
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
