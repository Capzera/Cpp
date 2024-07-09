#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, mx = -1;
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	for (int i = 0; i < n; i++) {
		if (a[i] != a[0]) {
			mx = max(mx, i);
		}
		if (a[n - 1] != a[i]) {
			mx = max(mx, n - 1 - i);
		}
	}
	cout << fixed << setprecision(3) << (1.0 / mx) << endl;
	return 0;
}
