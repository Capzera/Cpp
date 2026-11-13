#include <bits/stdc++.h>
using namespace std;
long long n, m, l, r, lg[50001], f1[50001][20], f2[50001][20];
int main() {
	cin >> n >> m;
	lg[0] = -1;
	memset(f2, 0x3f, sizeof(f2));
	for (int i = 1; i <= n; i++) {
		lg[i] = lg[i >> 1] + 1;
		cin >> f1[i][0];
		f2[i][0] = f1[i][0];
	}
	for (int j = 1; j <= lg[n]; j++) {
		for (int i = 1; i <= n - (1 << j) + 1; i++) {
			f1[i][j] = max(f1[i][j - 1], f1[i + (1 << j - 1)][j - 1]);
			f2[i][j] = min(f2[i][j - 1], f2[i + (1 << j - 1)][j - 1]);
		}
	}
	while (m--) {
		cin >> l >> r;
		int k = lg[r - l + 1];
		cout << max(f1[l][k], f1[r - (1 << k) + 1][k]) - min(f2[l][k], f2[r - (1 << k) + 1][k]) << endl;
	}
	return 0;
}
