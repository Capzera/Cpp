#include <bits/stdc++.h>
using namespace std;
long long n;
vector<vector<long long> > a;
bool check(long long x) {
	for (int i = 0; i < n; i++) {
		if (x + a[i][0] > a[i][1]) return 0;
		x += a[i][0];
	}
	return 1;
}
int main() {
	cin >> n;
	long long l = 0, r = 0;
	a = vector<vector<long long> > (n, vector<long long>(2));
	for (int i = 0; i < n; i++) {
		cin >> a[i][0] >> a[i][1];
		r = max(r, a[i][1] + 1);
	}
	sort(a.begin(), a.end(), [&](auto& x, auto& y) {
		return x[1] < y[1];
	});
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) l = mid + 1;
		else r = mid;
	}
	cout << l - 1;
	return 0;
}
