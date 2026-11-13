#include <bits/stdc++.h>
using namespace std;
long long n;
int main() {
	cin >> n;
	long long l = 1, r = 2e9, ans = 0;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (mid * log10(1.0 * mid) + 1 >= n) {
			r = ans = mid;
		} else l = mid + 1;
	}
	cout << ans;
	return 0;
}
