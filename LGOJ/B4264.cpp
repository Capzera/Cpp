#include <bits/stdc++.h>
using namespace std;
int n, m, ans, a[501][501];
int main() {
	cin >> n >> m;
 	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	for (int i = 1; i < n; i++)
		for (int j = 1; j < m; j++)
			if (a[i][j] * a[i + 1][j + 1] == a[i + 1][j] * a[i][j + 1]) ans++;
	cout << ans;
	return 0;
}
