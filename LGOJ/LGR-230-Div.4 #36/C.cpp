#include <bits/stdc++.h>
using namespace std;
long long n, ans = LLONG_MAX;
long long get(int x) {
	long long ans = 0;
	if (x < 11) return x * 20;
	if (x < 51) return 200 + (x - 6) / 5 * 80;
	return 840 + (x - 41) / 10 * 120;
}
int main() {
	cin >> n;
	for (int i = 0; i <= n; i++) {
		ans = min(ans, get(i) + get(n - i));
	}
	cout << ans;
	return 0;
}
