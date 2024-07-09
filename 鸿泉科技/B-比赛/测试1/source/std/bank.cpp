#include <bits/stdc++.h>
using namespace std;
int main () {
	long long m, n;
	cin >> m;
	long long l = 1, r = 1000001;
	while (l < r) {
		long long mid = l + (r - l) / 2;
		if (mid * (mid + 1) / 2 < m) {
			l = mid + 1;
		} else {
			r = mid;
		}
	}
	l--;
	int ans = (l + 1) * (2 * l + 1) * l / 6;
	ans += (m - l * (l + 1) / 2) * (l + 1);
	cout << ans;
}

