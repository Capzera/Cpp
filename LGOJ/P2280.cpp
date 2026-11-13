#include <bits/stdc++.h>
using namespace std;
long long n, m, x, y, w, a[5001][5001], ans;
int main() {
	cin >> n >> m;
	while (n--) {
		cin >> x >> y >> w;
		a[x + 1][y + 1] += w;
	}
	for (int i = 1; i <= 5000; i++) {
		for (int j = 1; j <= 5000; j++) {
			a[i][j] += a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1];
		}
	}
	for (int i = m; i <= 5000; i++) {
		for (int j = m; j <= 5000; j++) {
			ans = max(ans, a[i][j] - a[i][j - m] - a[i - m][j] + a[i - m][j - m]);
		}	
	}
	cout << ans;
	return 0;
}
