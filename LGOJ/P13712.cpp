#include <bits/stdc++.h>
using namespace std;
long long t, x, y, a, b, c, d;
long long solve() {
	cin >> x >> y >> a >> b >> c >> d;
	if (x == y) return 0;
	long long ans = LLONG_MAX;
	if ((x & a) == y) ans = min(ans, c);
	if ((x | b) == y) ans = min(ans, d);
	if (((x & a | b) == y) || (((x | b) & a) == y)) ans = min(ans, c + d);
	return ans == LLONG_MAX ? -1 : ans;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
