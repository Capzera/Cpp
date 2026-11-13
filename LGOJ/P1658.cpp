#include <bits/stdc++.h>
using namespace std;
long long x, n, sum, ans, a[11];
int main() {
	cin >> x >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	while (sum < x) {
		ans++;
		sum += *lower_bound(a + 1, a + n + 1, sum + 1);
	}
	cout << ans;
	return 0;
}
