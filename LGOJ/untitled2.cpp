#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 1e5 + 10;
struct ele {
	int val, loc;
	bool operator < (const struct ele& rhs) const {
		return val < rhs.val;
	}
} a[N], ans[N];

int n, p[N], l[N], r[N];

signed main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].val;
		a[i].loc = i;
	}
	sort(a + 1, a + n + 1);
	a[0].val = INT_MIN, a[n + 1].val = INT_MAX;
	for (int i = 1; i <= n; i++) {
		p[a[i].loc] = i;
		l[i] = i - 1, r[i] = i + 1;
	}
	for (int i = n; i > 1; i--) {
		int k = p[i], L = l[k], R = r[k];
		int lt = a[k].val - a[L].val;
		int rt = a[R].val - a[k].val;
		if (lt <= rt) {
			ans[i] = {lt, a[L].loc};
		} else {
			ans[i] = {rt, a[R].loc};
		}
		l[R] = L;
		r[L] = R;
	}
	for (int i = 2; i <= n; i++) cout << ans[i].val << " " << ans[i].loc << '\n';
	return 0;
}
