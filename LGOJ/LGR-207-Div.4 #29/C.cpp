#include <bits/stdc++.h>
using namespace std;
int n, k, d, a, mx, ans;
char ch;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> k >> ch >> d >> ch >> a;
		long long power = 0;
		if (k - d >= 10) power = k * (k - d) + a;
		else if (k >= d) power = (k - d + 1) * 3 + a;
		else power = 2 * a;
		if (power > mx) {
			mx = power;
			ans = i;
		}
	}
	cout << ans;
	return 0;
}
