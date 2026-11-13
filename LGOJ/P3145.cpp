#include <bits/stdc++.h>
using namespace std;
const int N = 50001;
struct point {
	int x, y;
} a[N];
bool cmp1(const point& x, const point& y) {
	return x.x < y.x;
}
bool cmp2(const point& x, const point& y) {
	return x.y < y.y;
}
long long n, ans = LLONG_MAX, lg[N], xmn[N][20], xmx[N][20], ymn[N][20], ymx[N][20];
long long query(int l, int r) {
	int k = lg[r - l + 1];
	long long xx = max(xmx[l][k], xmx[r - (1 << k) + 1][k]);
	long long xn = min(xmn[l][k], xmn[r - (1 << k) + 1][k]);
	long long yx = max(ymx[l][k], ymx[r - (1 << k) + 1][k]);
	long long yn = min(ymn[l][k], ymn[r - (1 << k) + 1][k]);
	return (xx - xn) * (yx - yn);
}
void rmq() {
	memset(xmn, 0x3f, sizeof(xmn));
	memset(ymn, 0x3f, sizeof(ymn));
	for (int i = 1; i <= n; i++) xmn[i][0] = xmx[i][0] = a[i].x;
	for (int i = 1; i <= n; i++) ymn[i][0] = ymx[i][0] = a[i].y;
	for (int j = 1; j <= lg[n]; j++) {
		for (int i = 1; i <= n - (1 << j) + 1; i++) {
			xmx[i][j] = max(xmx[i][j - 1], xmx[i + (1 << j - 1)][j - 1]);
			xmn[i][j] = min(xmn[i][j - 1], xmn[i + (1 << j - 1)][j - 1]);
			ymx[i][j] = max(ymx[i][j - 1], ymx[i + (1 << j - 1)][j - 1]);
			ymn[i][j] = min(ymn[i][j - 1], ymn[i + (1 << j - 1)][j - 1]);
		}
	}
}
void solve() {
	for (int i = 1; i < n; i++) {
		ans = min(ans, query(1, i) + query(i + 1, n));
	}
}
int main() {
	cin >> n;
	lg[0] = -1;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].y;
		lg[i] = lg[i >> 1] + 1;
	}
	sort(a + 1, a + n + 1, cmp1);
	rmq();
	solve();
	sort(a + 1, a + n + 1, cmp2);
	rmq();
	solve();
	cout << query(1, n) - ans;
}
