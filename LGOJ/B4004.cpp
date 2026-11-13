#include <bits/stdc++.h>
using namespace std;
int t, n, a[100010];
bool solve() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = n - 1; i > 0; i--) {
		if (a[n] % a[i]) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes\n" : "No\n");
	}
	return 0;
}
