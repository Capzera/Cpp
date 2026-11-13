#include <bits/stdc++.h>
using namespace std;
int l, r, ans;
bool isPrime(int x) {
	int n = sqrt(x);
	if (x < 2) return 0;
	if (x < 4) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
int main() {
	cin >> l >> r;
	for (int i = l; i <= r; i++) {
		ans += isPrime(i);
	}
	cout << ans;
	return 0;
}
