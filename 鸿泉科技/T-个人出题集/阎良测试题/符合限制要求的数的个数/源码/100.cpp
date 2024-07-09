#include <bits/stdc++.h>
using namespace std;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int a, b, c, x, y, ans;
	while (cin >> a >> b >> c >> x >> y, a >= 0) {
		a += x, c -= y;
		ans = max(0, c - a + 1 - (b >= a && b <= c));
		cout << ans << endl;
	}
	return 0;
}
