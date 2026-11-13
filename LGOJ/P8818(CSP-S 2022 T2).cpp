#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 100001;
long long n, m, q, x, l1, r1, l2, r2, a[N], b[N];
int amn[N][20], amx[N][20], bmn[N][20], bmx[N][20], azmn[N][20], afmx[N][20];
short lg[N];
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
signed main() {
	n = read(), m = read(), q = read();
	lg[0] = -1;
	for (int i = 1; i <= max(n, m); i++) lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; i++) {
		x = read();
		amx[i][0] = amn[i][0] = x;
		azmn[i][0] = x >= 0 ? x : LLONG_MAX;
		afmx[i][0] = x < 0 ? x : LLONG_MIN;
	}
	for (int i = 1; i <= m; i++) {
		x = read();
		bmn[i][0] = bmx[i][0] = x;
	}
	for (int j = 1; j <= lg[n]; j++) {
		for (int i = 1; i <= n - (1 << j) + 1; i++) {
			amn[i][j] = min(amn[i][j - 1], amn[i + (1 << j - 1)][j - 1]);
			amx[i][j] = max(amx[i][j - 1], amx[i + (1 << j - 1)][j - 1]);
			azmn[i][j] = min(azmn[i][j - 1], azmn[i + (1 << j - 1)][j - 1]);
			afmx[i][j] = max(afmx[i][j - 1], afmx[i + (1 << j - 1)][j - 1]);
		}
	}
	for (int j = 1; j <= lg[m]; j++) {
		for (int i = 1; i <= m - (1 << j) + 1; i++) {
			bmn[i][j] = min(bmn[i][j - 1], bmn[i + (1 << j - 1)][j - 1]);
			bmx[i][j] = max(bmx[i][j - 1], bmx[i + (1 << j - 1)][j - 1]);
		}
	}

	while (q--) {
		l1 = read(), r1 = read(), l2 = read(), r2 = read();
		int k1 = lg[r1 - l1 + 1], k2 = lg[r2 - l2 + 1];
		int xmx = max(amx[l1][k1], amx[r1 - (1 << k1) + 1][k1]);
		int xmn = min(amn[l1][k1], amn[r1 - (1 << k1) + 1][k1]);
		int xfmx = max(afmx[l1][k1], afmx[r1 - (1 << k1) + 1][k1]);
		int xzmn = min(azmn[l1][k1], azmn[r1 - (1 << k1) + 1][k1]);
		int ymx = max(bmx[l2][k2], bmx[r2 - (1 << k2) + 1][k2]);
		int ymn = min(bmn[l2][k2], bmn[r2 - (1 << k2) + 1][k2]);
		int ans = LLONG_MIN;
		ans = max(ans, xmx * (xmx >= 0 ? ymn : ymx));
		ans = max(ans, xmn * (xmn >= 0 ? ymn : ymx));
		if (xfmx != LLONG_MIN) ans = max(ans, xfmx * (xfmx >= 0 ? ymn : ymx));
		if (xzmn != LLONG_MAX) ans = max(ans, xzmn * (xzmn >= 0 ? ymn : ymx));
		printf("%lld\n", ans);
	}
	return 0;
}
/*

A的行为，
选择一个 x >= 0
	y 负数 x最小，y最小
	y 正数 x最大，y最小
	y 正负 x最小，y最小
选择一个 x < 0
	y 负数 x最小，y最大
	y 正数 x最大，y最大
	y 正负 x最大，y最大
*/
