#include <bits/stdc++.h>
using namespace std;
long long n, x, ans = 0;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		ans = __gcd(ans, abs(x));
	}
	cout << ans;
	return 0;
}
