#include <bits/stdc++.h>
using namespace std;
long long n, s, t, ans, a[51], b[51];
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	cin >> s >> t;
	for (int i = s; i <= t; i++) {
		ans += a[i] + b[i];
	}
	cout << ans - a[t];
	return 0;
}
