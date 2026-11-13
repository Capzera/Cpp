#include <bits/stdc++.h>
using namespace std;
long long t, n, x;

long long solve() {
	cin >> n >> x;
	return n / x + n;
}

int main() {
	cin >> t;
	while (t--) {
		cout << solve() << '\n';
	}
	return 0;
}
