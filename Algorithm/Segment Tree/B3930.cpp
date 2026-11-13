#include <bits/stdc++.h>
using namespace std;
int n, a[1000010], ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1, greater<int>());
	n = min(n, 32);
	for (int i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			ans = max(ans, a[i] & a[j]);
		}
	}
	cout << ans;
	return 0;
}
