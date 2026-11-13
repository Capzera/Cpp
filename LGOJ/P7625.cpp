#include <bits/stdc++.h>
using namespace std;
long long n, l = 1, r, q, x, ans;
int main() {
	cin >> n >> r >> q;
	while (q--) {
		cin >> x;
		if (x < l) ans += l - x, r -= l - x, l = x;
		if (x > r) ans += x - r, l += x - r, r = x;
	}
	cout << ans;
	return 0;
}
