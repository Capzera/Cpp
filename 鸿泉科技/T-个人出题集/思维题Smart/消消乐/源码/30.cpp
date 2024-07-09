#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, ans = INT_MIN;
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			a[j] -= a[i];
		}
		ans = max(ans, a[i + 1]);
	}
	cout << ans << endl;
	return 0;
}
