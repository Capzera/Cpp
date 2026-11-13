#include <bits/stdc++.h>
using namespace std;
int n, a[100000][3], f[4];
inline int read() {
	int ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = (ans << 3) + (ans << 1) + ch - '0';
		ch = getchar();
	}
	return ans;
}
int main() {
	n = read();
	for (int i = 0; i < n; i++) a[i][0] = read(), a[i][1] = read(), a[i][2] = read();
	for (int i = 0; i < 3; i++) a[n][i] = a[0][i];
	for (int i = 1; i <= n; i++) {
		auto [f0, f1, f2, f3] = f;
		f[0] = max(f2, f3) + a[i][0];
		f[1] = f3 + a[i][1];
		f[2] = f0 + a[i][1];
		f[3] = max(f0, f1) + a[i][2];
	}
	cout << *max_element(f, f + 4);
	return 0;
}
/*
nxt[i][j][k] 位置i，种的树为j，高度为k=1(高),k=0,(低) 能获得的最大得分
nxt[1][1][0] = nxt[1][1][1] = a[1][1];
nxt[1][2][0] = nxt[1][2][1] = a[1][2]
nxt[1][3][0] = nxt[1][3][1] = a[1][3];

max(nxt[n][1..3][0..1])
6
1 3 2
3 1 2
1 2 3
1 3 2
2 1 3
2 3 2
 2 1 3 2 3 2
*/
