#include <bits/stdc++.h>
using namespace std;
int n, m, k, a[101][101], ans;
char x;
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> x;
			a[i][j] = x == '.';
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			int f = 1, cnt = 0;
			for (int l = 0; l < k; l++) {
				if (j + l > m || !a[i][j + l]) {
					f = 0;
					break;
				}
			}
			ans += f;
			f = 1;
			for (int l = 0; l < k; l++) {
				if (i + l > n || !a[i + l][j]) {
					f = 0;
					break;
				}
			}
			ans += f;
		}
	}
	if (k == 1) ans /= 2;
	cout << ans;
	return 0;
}
