#include <bits/stdc++.h>
using namespace std;
int t, n, m, r1, r2, c1, c2;
bool chk(int x, int y) {
	return x >= 1 && x <= n && y >= 1 && y <= m;
}
int solve() {
	cin >> n >> m >> r1 >> c1 >> r2 >> c2;
	if (r1 == r2 && c1 == c2) return 0;
	if (max(abs(r1 - r2), abs(c1 - c2)) == 1) return 1;
	if (r1 == r2 && )
	return 3;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
