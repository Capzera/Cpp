#include <bits/stdc++.h>
using namespace std;
long long n, k = 1e9, ans;
struct node {
	int l, r;
	bool operator < (const struct node& rhs) const {
		if (l != rhs.l) return l > rhs.l;
		return r > rhs.r;
	}
} a[100005];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].l >> a[i].r;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (a[i].r >= k) continue;
		k = a[i].l;
		ans += k;
	}
	cout << ans;
	return 0;
}

