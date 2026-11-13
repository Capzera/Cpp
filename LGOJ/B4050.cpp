#include <bits/stdc++.h>
using namespace std;
int t = 1, n;
bool isPrime(int x) {
	if (x < 2) return 0;
	if (x < 4) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	int n = sqrt(x);
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
int solve() {
	int ans = 0;
	cin >> n;
	if (n == 1 || isPrime(n)) return 1;
	for (int i = 1, j = 1; n >= 0; i++, j *= 2) {
		if (isPrime(n - j)) return i + 1;
		n -= j;
	}
	return -1;
}
int main() {
	cin >> t;
	while (t--) {
		cout << solve() << endl;
	}
	return 0;
}
