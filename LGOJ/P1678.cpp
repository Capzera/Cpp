#include <bits/stdc++.h>
using namespace std;
int m, n, a[100002], x, loc;
long long ans;
int main() {
	cin >> m >> n;
	for (int i = 1; i <= m; i++) cin >> a[i];
	sort(a + 1, a + m + 1);
	a[0] = -1e7, a[m + 1] = 1e7;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		loc = lower_bound(a + 1, a + m + 1, x) - a;
		ans += min(abs(a[loc] - x), abs(x - a[loc - 1]));
	}
	cout << ans;
	return 0;
}
