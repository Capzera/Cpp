#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	cin >> n >> k;
	vector<long long> a(n), ans(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 1; j <= abs(k); j++) {
			int x = k > 0 ? (i + j) % n : (i - j + n) % n;
			ans[i] += a[x];
		}
	}
	for (auto& x : ans) cout << x << " ";
	return 0;
}

