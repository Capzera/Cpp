#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
long long n, sum, x, f, g, mx = LLONG_MIN, mn = LLONG_MAX;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		sum += x;
		mx = max(mx, x);
		f = x + max(0ll, f);
		g = x + min(0ll, g);
		mx = max(mx, f);
		mn = min(mn, g);
	}
	cout << (mx < 0 ? mx : max(mx, sum - mn));
	return 0;
}
