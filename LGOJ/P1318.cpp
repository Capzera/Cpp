#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	int l = 0, r = n - 1, ml = a[0], mr = a.back();
	while (l < r) {
		ml = max(ml, a[l]);
		mr = max(mr, a[r]);
		if (a[l] < a[r]) {
			ans += ml - a[l++];
		} else ans += mr - a[r--];
	}
	cout << ans;
	return 0;
}
