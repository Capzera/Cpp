#include <bits/stdc++.h>
using namespace std;

int isPrime(int x) {
	int m = sqrt(x);
	if (x < 4) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	for (int i = 5; i <= m; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0) return 0;
	}
	return 1;
}
int t, n;
int solve() {
	cin >> n;
	for (int i = 0; i <= 6; i++) {
		if (isPrime(n + i)) continue;
		return n + i;
	}
}
int main() {
	cin >> t;
	while (t--) cout << solve() << "\n";
	
	return 0;
}
