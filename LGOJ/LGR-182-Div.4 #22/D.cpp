#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, x, mn = LLONG_MAX, mx = LLONG_MIN;
	cin >> n;
	while (n--) {
		cin >> x;
		mn = min(mn, x);
		mx = max(mx, x);
	}
	cout << mn + mx;
	return 0;
}
