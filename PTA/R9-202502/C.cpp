#include <bits/stdc++.h>
using namespace std;
const int N = 5e4 + 10;
int n, k, L, a[N];
bool check(int x) {
	int cnt = 0;
	for (int i = 1, lst = 0; i <= n + 1; i++) {
		if (a[i] - a[lst] < x) {
			if (++cnt > k) return 0;
		} else lst = i;
	}
	return cnt <= k;
}
int main() {
	cin >> L >> n >> k;
	int l = 1, r = L + 1, ans = L;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		
	}
	a[n + 1] = L;
	
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans;
	return 0;
}
