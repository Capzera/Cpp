#include <bits/stdc++.h>
using namespace std;
long long n, x, m, ans, a[101];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cin >> x;
	for (int i = 1; i <= n; i++) {
		if (x >= a[i]) {
			ans++;
			x -= a[i];
		}
	}
	cout << ans;
	return 0;
}
