#include <bits/stdc++.h>
using namespace std;
long long n, m, ans, a[1001][1001], sum[1001][1001];
int main() {
	freopen("bean.in", "r", stdin);
	freopen("bean.out", "w", stdout);
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> a[i][j];
			sum[i + 1][j + 1] = sum[i][j + 1] + sum[i + 1][j] - sum[i][j] + a[i][j];
		}
	}
	cin >> m;
	m = min(m, n - 1);
	for (int i = 1; i <= n - m; i++)
		for (int j = 1; j <= n - m; j++)
			ans = max(ans, sum[i + m][j + m] - sum[i + m][j - 1] - sum[i - 1][j + m] + sum[i - 1][j - 1]);
	cout << ans << endl;
	return 0;
}
