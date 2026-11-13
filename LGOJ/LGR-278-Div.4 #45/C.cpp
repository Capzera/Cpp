#include <bits/stdc++.h>
using namespace std;
long long n, k, mx, a[100010];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int i = 1; i <= n; i++) {
		if (i <= k) {
			mx = max(mx, a[i]);
		} else {
			if (a[i] > mx) {
				return cout << a[i], 0;
			}
		}
	}
	cout << a[n];
	return 0;
}
