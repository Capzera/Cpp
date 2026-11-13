#include <bits/stdc++.h>
using namespace std;
long long n, x, ans = LLONG_MIN, mn;
int main() {
	cin >> n >> mn;
	for (int i = 2; i <= n; i++) {
		cin >> x;
		ans = max(ans, x - mn);
		mn = min(mn, x);
	}
	cout << ans;
	return 0;
}
