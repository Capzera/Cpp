#include <bits/stdc++.h>
using namespace std;
int n, a[200010], b[200010], c[200010], x[200010], y[200010], z[200010];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> b[i] >> c[i];
		x[i] = a[i], y[i] = b[i], z[i] = c[i];
	}
	sort(a + 1, a + n + 1, greater<int>());
	sort(b + 1, b + n + 1, greater<int>());
	sort(c + 1, c + n + 1, greater<int>());
	for (int i = 1; i <= n; i++) {
		int l1 = lower_bound(a + 1, a + n + 1, x[i], greater<int>()) - a;
		int l2 = lower_bound(b + 1, b + n + 1, y[i], greater<int>()) - b;
		int l3 = lower_bound(c + 1, c + n + 1, z[i], greater<int>()) - c;
		cout << min(l1, min(l2, l3)) << endl;
	}
	return 0;
}
