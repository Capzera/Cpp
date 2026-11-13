#include <bits/stdc++.h>
using namespace std;
long long n, m, ans = LLONG_MAX, a[100010];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i + m - 1 <= n; i++) {
		ans = min(ans, a[i + m - 1] * a[i + m - 1] - a[i] * a[i]);
	}
	cout << ans;
	return 0;
}
