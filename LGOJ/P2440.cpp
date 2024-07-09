#include <bits/stdc++.h>
using namespace std;
int n, k, a[100001], mx = 0;
bool check(int x) {
	if (!x) return 1;
	long long sum = 0;
	for (int i = 0; i < n; i++) {
		sum += a[i] / x;
		if (sum >= k) return 1;
	}
	return 0;
}
int main() {
	cin >> n >> k;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		mx = max(mx, a[i]);
	}
	int l = 0, r = mx + 1;
	while (l < r) {
		int mid = l + (r - l) / 2;
		if (check(mid)) l = mid + 1;
		else r = mid;
	}
	cout << l - 1;
	return 0;
}
