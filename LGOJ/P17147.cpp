#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
long long t, n, k, a[N], b[N];
int solve() {
	cin >> n >> k;
	int ans = 0;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	sort(a + 1, a + n + 1);
	sort(b + 1, b + n + 1, greater<long long>());
	for (int i = 1, j = 1; i <= n && j <= n; i++) {
		if (a[i] + b[j] >= k) ans++, j++;
	}
	return ans;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
