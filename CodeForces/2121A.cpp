#include <bits/stdc++.h>
using namespace std;
long long n, s, ans, a[11];
void solve() {
	cin >> n >> s;
	for (int i = 1; i <= n; i++) cin >> a[i];
	if (s < a[1]) ans = a[n] - s;
	else if (s > a[n]) ans = s - a[1];
	else ans = a[n] - a[1] + min(s - a[1], a[n] - s);
	cout << ans << endl;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) solve();
	return 0;
}
