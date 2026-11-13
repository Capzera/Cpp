#include <bits/stdc++.h>
using namespace std;
string sn, sx, sy;
int k, n, m, ans, x, y, a[10][10], b[10][10];
int main() {
	cin >> k;
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			cin >> a[i][j];
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			cin >> b[i][j];
	cin >> sn >> sx >> sy;
	for (auto& ch : sn) n = n * k + ch - '0';
	for (auto& ch : sx) x = x * k + ch - '0';
	for (auto& ch : sy) y = y * k + ch - '0';
	for (int i = 1; i <= n; i++) {
		int va = 0, vo = 0;
		for (int mask = i, val = x, p = 1; mask || val; mask /= k, val /= k, p *= k) {
			va += a[mask % k][val % k] * p;
			vo += b[mask % k][val % k] * p;
		}
		if (va == y || vo == y) ans++;
	}
	cout << ans;
	return 0;
}
