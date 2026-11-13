#include <bits/stdc++.h>
using namespace std;
long long n, p, st, ed, mn = 1000, ans;
int main() {
	cin >> n >> p;
	st = pow(10, n - 1), ed = 10 * st - 1;
	for (int i = st; i <= ed; i++) {
		int j = i, cnt = 0;
		while (j) {
			cnt += j % 10;
			j /= 10;
		}
		if (cnt % p < mn) {
			mn = cnt % p;
			ans = i;
		}
		if (!mn) break;
	}
	cout << ans;
	return 0;
}
/*
n <= 7, p <= 100
*/
