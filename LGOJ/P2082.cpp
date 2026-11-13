#include <bits/stdc++.h>
using namespace std;
struct interval {
	long long l, r;
	bool operator < (const struct interval& rhs) const {
		if (l != rhs.l) return l < rhs.l;
		return r < rhs.r;
	}
} a[100001];
long long m, n, ans, lst = -1;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].l >> a[i].r;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		long long l = a[i].l, r = a[i].r;
		if (l > lst) {
			ans += r - l + 1;
		} else if (r > lst) ans += r - max(l, lst);
		lst = max(lst, r);
	}
	cout << ans << endl;
	return 0;
}
