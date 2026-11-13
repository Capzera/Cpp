#include <bits/stdc++.h>
using namespace std;
long long n, m, l, r, lg[100001], f[100001][21];
inline int read() {
	int x = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		x = (x << 3) + (x << 1) + ch - 48;
		ch = getchar();
	}
	return x * f;
}
int main() {
	cout.tie(nullptr)->sync_with_stdio(0);
	n =read(), m = read();
	lg[1] = 0;
	for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; i++) f[i][0] = read();
	for (int j = 1; j <= lg[n]; j++)
		for (int i = 1; i <= n - (1 << j) + 1; i++) {
			f[i][j] = max(f[i][j - 1], f[i + (1 << j - 1)][j - 1]);
		}
	while (m--) {
		l = read(), r = read();
		int k = lg[r - l + 1];
		cout << max(f[l][k], f[r - (1 << k) + 1][k]) << '\n';
	}
	return 0;
}
