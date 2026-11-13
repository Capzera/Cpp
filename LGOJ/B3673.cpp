#include <bits/stdc++.h>
using namespace std;
long long n, c, a[1000010], b, ans;
int main() {
	cin >> n >> c;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		cin >> b;
		ans += max(b - a[i], 0ll) * c;
	}
	cout << ans;
	return 0;
}
