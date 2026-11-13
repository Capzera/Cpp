#include <bits/stdc++.h>
using namespace std;
long long n, m, k, a[101][101], b[101][101];
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) 
			cin >> a[i][j];
	for (int i = 1; i <= k; i++) 
		for (int j = 1; j <= k; j++)
			cin >> b[i][j];
	for (int i = 1; i <= n - k + 1; i++) {
		for (int j = 1; j <= m - k + 1; j++) {
			long long sum = 0;
			for (int x = 1; x <= k; x++) {
				for (int y = 1; y <= k; y++) {
					sum += a[i + x - 1][j + y - 1] * b[x][y];
				}
			}
			cout << sum << " ";
		}
		cout << endl;
	}
	return 0;
}
