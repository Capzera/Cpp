#include <bits/stdc++.h>
using namespace std;
long long n, a[100010], ans = INT_MAX;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		ans = min(ans, max(a[i], a[i + 1]) - min(a[i], a[i + 1]));
	}
	cout << ans;
	return 0;
}
