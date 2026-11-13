#include <bits/stdc++.h>
using namespace std;
long long f(int x) {
	long long ans = 0;
	int st = 0;
	while (st + 3 <= x) {
		int k = st / 4;
		ans += 8 * k + 3;
		st += 4;
	}
	for (; st <= x; st++) {
		if (__builtin_popcount(st) & 1) ans += st;
	}
	return ans;
}
int l, r;
int main() {
	cin >> l >> r;
	cout << f(r) - f(l - 1);
	return 0;
}
