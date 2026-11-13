#include <bits/stdc++.h>
using namespace std;
int n, t[501], ans;
struct node {
	int t, r;
	bool operator < (const struct node& rhs) {
		if (t != rhs.t) return t < rhs.t;
		return r > rhs.r;
	}
} a[501];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].t;
	for (int i = 1; i <= n; i++) cin >> a[i].r;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		int ans = INT_MAX, k = a[i].t;
		for (int j = 1; j <= a[i].t; j++) {
			if (t[j] < ans) {
				ans = t[j];
				k = j;
			}
		}
		t[k] = max(t[k], a[i].r);
	}
	for (int i = 1; i <= a[n].t; i++) {
		ans += t[i];
	}
	cout << ans;
	return 0;
}
