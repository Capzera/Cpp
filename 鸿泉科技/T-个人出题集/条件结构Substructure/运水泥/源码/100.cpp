#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, a, k, b, ans = 0;
	cin >> n >> a >> k >> b;
	if (n % 2) {
		cout << -1;
		return 0;
	}
	n /= 2;
	if (b <= a * k) {
		ans = n / k * b * 2;
		ans += min(n % k * a, b) * 2;
	} else ans = n * 2 * a;
	cout << ans;
	return 0;
}
