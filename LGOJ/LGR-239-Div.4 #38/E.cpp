#include <bits/stdc++.h>
using namespace std;
long long n, ans, x, a[100001], b[100001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = 1; i <= n; i++) {
		cin >> x;
		ans += max(0ll, x - abs(a[i] - b[i]));
	}
	cout << ans;
	return 0;
}
