#include <bits/stdc++.h>
using namespace std;
long long n, mn = LLONG_MAX, sum, a[100001];
double mx;
int main() {
	cin >> n;
	vector<int> ans;
	for (int i = n; i > 0; i--) cin >> a[i];
	for (int i = 1; i < n; i++) {
		sum += a[i];
		mn = min(mn, a[i]);
		if (i == 1) continue;
		if (sum - mn > mx * (i - 1)) {
			mx = 1.0 * (sum - mn) / (i - 1);
			ans.clear();
		}
		if ((1.0 * sum - mn) / (i - 1) == mx) {
			ans.push_back(n - i);
		}
	}
	for (int i = ans.size() - 1; ~i; i--) cout << ans[i] << endl;
	return 0;
}
