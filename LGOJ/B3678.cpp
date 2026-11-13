#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, m, ans, a[N], b[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = 1; i <= n; i++) {
		if (m < a[i]) continue;
		m += b[i];
		ans++;
	}
	cout << ans;
	return 0;
}
