#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, q, l, r, x, pre[N][4];
inline int read() {
	int res = 0;
	char ch = getchar();
	while (!isdigit(ch)) {
		ch = getchar();
	}
	while (isdigit(ch)) {
		res = res * 10 + ch - '0';
		ch = getchar();
	}
	return res;
}
int main() {
	n = read(), q = read();
	for (int i = 1; i <= n; i++) {
		x = read();
		for (int j = 1; j <= 3; j++) pre[i][j] = pre[i - 1][j] + (x == j);
	}
	while (q--) {
		l = read(), r = read();
		for (int j = 1; j <= 3; j++) printf("%d ", pre[r][j] - pre[l - 1][j]);
		printf("\n");
	}
	return 0;
}
