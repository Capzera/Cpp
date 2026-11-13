#include <bits/stdc++.h>
using namespace std;
int n, k, ans, a[101];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	while (1) {
		int mx = 0, mn = 101, mxl = 0;
		for (int i = n; i > 0; i--) {
			if (!a[i]) continue;
			if (a[i] > mx) {
				mx = a[i];
				mxl = i;
			}
			mn = min(a[i], mn);
		}
		if (!mx) break;
		a[mxl] -= mn;
		ans++;
	}
	cout << ans;
}
