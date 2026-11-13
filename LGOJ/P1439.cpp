#include <bits/stdc++.h>
using namespace std;
int n, x;
int main() {
	cin >> n;
	vector<int> a(n), r(n + 1), ans;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		r[x] = i;
	}
	for (auto& num : a) {
		cin >> x;
		num = r[x];
	}
	for (auto& x : a) {
		if (ans.empty() || x > ans.back()) {
			ans.emplace_back(x);
			continue;
		}
		int pos = upper_bound(ans.begin(), ans.end(), x) - ans.begin();
		ans[pos] = x;
	}
	cout << ans.size() << endl;
	return 0;
}

