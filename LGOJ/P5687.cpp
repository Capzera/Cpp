#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 5;
long long n, m, a[N], b[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int j = 1; j <= m; j++) cin >> b[j];
	sort(a + 1, a + n + 1), sort(b + 1, b + m + 1);
	long long ans = a[1] * (m - 1) + b[1] * (n - 1), i = 2, j = 2;
	while (i <= n && j <= m) {
		if (a[i] <= b[j]) ans += a[i++] * (m - j + 1);
		else ans += b[j++] * (n - i + 1);
	}
	cout << ans;
	return 0;
}
