#include <bits/stdc++.h>
using namespace std;
long long n, x, ans = LLONG_MIN;
int main() {
	cin >> n;
	vector<long long> row(n), col(n), d1(2 * n), d2(2 * n + 1);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> x;
			row[i] += x;
			col[j] += x;
			d1[i + j] += x;
			d2[i - j + n - 1] += x;
		}
	}
	for (int i = 0; i < n; i++) {
		ans = max(ans, max(row[i], col[i]));
	}
	for (int i = 0; i < 2 * n - 1; i++) {
		ans = max(ans, max(d1[i], d2[i]));
	}
	cout << ans;
	return 0;
}
