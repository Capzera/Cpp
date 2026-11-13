#include<bits/stdc++.h>
using namespace std;
long long n, k, ans;
bool check(vector<long long> &a, long long x) {
	long long cnt = 0;
	for (int l = 1, r = n; l <= n; l++) {
		if (l > r) r = l;
		while (l <= r && a[l] + a[r] >= x) r--;
		cnt += n - r;
	}
	return cnt < k;
}
int main() {
	cin >> n >> k;
	vector<long long> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a.begin() + 1, a.end());
	long long l = a[1] << 1, r = a[n] << 1 | 1;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(a, mid)) r = mid;
		else ans = mid, l = mid + 1;
	}
	if (ans % 2 != 0) cout << ans << endl << 2 << endl;
	else cout << ans / 2 << endl << 1 << endl;
	return 0;
}
