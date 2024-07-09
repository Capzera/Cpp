#include <bits/stdc++.h>
using namespace std;
int a[200010];
int main() {
	int n, k;
	cin >> n >> k;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	sort(a, a + n);
	int l = 0, r = n - 1;
	while (l <= r) {
		int mid = l + (r - l ) / 2;
		if (a[mid] > k) r = mid - 1;
		if (a[mid] < k) l = mid + 1;
		if (a[mid] == k) {
			cout << mid;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
