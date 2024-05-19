#include <bits/stdc++.h>
using namespace std;
long long x, y;
long long cal(long long n) {
	long long ans = 0;
	for (long long l = 1, r = 1; l <= n; l = r + 1) {
		r = n / (n / l);
		ans += 1ll * (n / l) * (r - l + 1) * (l + r) / 2;
	}
	return ans;
}
int main() {
	cin >> x >> y;
	cout << cal(y) - cal(x - 1);
	return 0;
}
