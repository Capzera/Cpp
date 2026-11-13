#include <bits/stdc++.h>
using namespace std;
long long m, d, x, n, t, f[200001][20], lg[200001];
char op;
int main() {
	cin >> m >> d;
	lg[0] = -1;
	for (int i = 1; i <= m; i++) lg[i] = lg[i >> 1] + 1;
	while (m--) {
		cin >> op >> x;
		if (op == 'A') {
			f[++n][0] = (x + t) % d;
			for (int j = 1; j <= lg[n]; j++) {
				f[n][j] = max(f[n][j - 1], f[n - (1 << j - 1)][j - 1]);
			} 
		} else {
			int r = n, l = r - x + 1, k = lg[x];
			t = max(f[r][k], f[l + (1 << k) - 1][k]);
			cout << t << endl;
		}
	}
	return 0;
}
