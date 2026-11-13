#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, k, ans, a[N];
void merge_sort(int l, int r) {
	int tmp[r - l + 1] = {0};
	if (l >= r) return;
	int mid = l + (r - l >> 1);
	merge_sort(l, mid), merge_sort(mid + 1, r);
	for (int i = l, j = mid + 1; i <= mid; i++) {
		while (j <= r && a[i] - a[j] > k) j++;
		ans += j - mid - 1;
	}
	int i = l, j = mid + 1, k = 0;
	while (i <= mid && j <= r) tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
	while (i <= mid) tmp[k++] = a[i++];
	while (j <= r) tmp[k++] = a[j++];
	for (int i = l; i <= r; i++) a[i] = tmp[i - l];
}
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	merge_sort(1, n);
	cout << ans;
	return 0;
}
