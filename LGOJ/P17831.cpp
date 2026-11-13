#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 5;
long long n, m, sum, ans, a[N];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (long long l = 1, r = 1; r <= n; r++) {
		sum += a[r];
		while (a[r] * (r - l + 1) - sum > m) sum -= a[l++];
		ans = max(ans, r - l + 1);
	}
	cout << ans;
	return 0;
}

