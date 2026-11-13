#include <bits/stdc++.h>
using namespace std;
int n, m, f[2000001][22];
short lg[2000001];
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
	n = read(), m = read();
	memset(f, 0x3f, sizeof(f));
	lg[1] = f[0][0] = 0;
	for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; i++) f[i][0] = read();
	for (int j = 1; j <= lg[n]; j++) {
		for (int i = 0; i <= n - (1 << j) + 1; i++) {
			f[i][j] = min(f[i][j - 1], f[i + (1 << j - 1)][j - 1]);
		}
	}
	for (int i = 1; i <= n; i++) {
		int l = max(i - m, 1), r = i - 1, k = lg[r - l + 1];
		cout << min(f[l][k], f[r - (1 << k) + 1][k]) << '\n';
	}
	return 0;
}
