#include <bits/stdc++.h>
using namespace std;
long long n, m, k, a[51][51], col[51];
void op() {
	long long mx = INT_MIN, mn = LLONG_MAX;
	for (int j = 1; j <= m; j++) {
		col[j] = 0;
		for (int i = 1; i <= n; i++) col[j] += a[i][j];
		mx = max(mx, col[j]);
		mn = min(mn, col[j]);
	}
	vector<long long> vec;
	for (int j = 1; j <= m; j++) {
		if (col[j] != mx && col[j] != mn) continue;
		for (int i = 1; i <= n; i++) {
			vec.push_back(a[i][j]);
			a[i][j] = 0;
		}
	}
	sort(vec.begin(), vec.end(), greater<long long>());
	for (int i = 1, k = 0; i <= n; i++) {
		if (i & 1) {
			for (int j = 1; j <= m; j++) {
				if (a[i][j]) continue;
				a[i][j] = vec[k++];
			}
		} else {
			for (int j = m; j > 0; j--) {
				if (a[i][j]) continue;
				a[i][j] = vec[k++];
			}
		}
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	while (k--) op();
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) cout << a[i][j] << " ";
		cout << endl;
	}
	return 0;
}
