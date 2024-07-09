#include <bits/stdc++.h>
using namespace std;
bool check(long long x) {
	if (x == 1) return 0;
	if (x == 2 || x == 3) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	long long n = sqrt(x);
	for (long long i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
int main() {
	long long n, x, ans = 0;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		if (check(x)) ans ^= i;
	}
	cout << ans << endl;
	return 0;
}
