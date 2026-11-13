#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;
int n, a[N], b[N], c[N];
int solve() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	sort(a + 1, a + n + 1);
	for (int i = 0; i <= n; i++) {
		
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) cout << solve() ? "YES\n" : "NO\n";
	return 0;
}
