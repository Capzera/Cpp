#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, d, x, mn = INT_MAX, ans = 0, cnt, last = 0;
	cin >> n >> d;
	vector<int> v(n);
	for (int i = 1; i < n; i++) cin >> v[i];
	for (int i = 1; i < n; i++) {
		cin >> x;
		mn = min(mn, x);
		cnt = (v[i] - last + d - 1) / d;
		last = cnt * d - v[i] + last;
		ans += cnt * mn;
	}
	cout << ans << endl;
	return 0;
}
