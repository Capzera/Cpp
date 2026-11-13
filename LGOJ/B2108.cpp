#include <bits/stdc++.h>
using namespace std;
long long a[101][101], n, m, ans[101][101];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (i == 1 || j == 1 || i == n || j == m) {
				ans[i][j] = a[i][j];
				continue;
			}
			ans[i][j] = round(1.0 * (a[i][j] + a[i - 1][j] + a[i + 1][j] + a[i][j - 1] + a[i][j + 1]) / 5);
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cout << ans[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
