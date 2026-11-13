#include <bits/stdc++.h>
using namespace std;
int n, a[100010], mx = INT_MIN, mn = INT_MAX, k;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mx = max(mx, a[i]);
		mn = min(mn, a[i]);
	}
	for (int i = 1; i <= n; i++) {
		cout << (a[i] < k ? mn : a[i] > k ? mx : k) << " ";
	}
	return 0;
}
