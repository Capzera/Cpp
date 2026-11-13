#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int t, n, lg[N], f[N][21];
inline int read() {
	int x = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		x = (x << 1) + (x << 3) + (ch ^ 48);
		ch = getchar();
	}
	return x;
}
int query(int l, int r) {
	int k = lg[r - l + 1];
	return max(f[l][k], f[r - (1 << k) + 1][k]);
}
bool check(long long x, vector<int> b) {
	long long cnt = 0;
	for (int i = 1, j = 1; i <= n; i++) {
		long long need = x;
		if (cnt >= x) cnt -= x, need = 0;
		else need -= cnt, cnt = 0;
		while (j <= i && need > 0) {
			long long mx = query(j, i);
			long long cost = min((need + mx - 1) / mx, 1ll * b[j]);
			need -= cost * mx;
			b[j] -= cost;
			if (!b[j]) j++;
		}
		if (need > 0) return 0;
		cnt += -need;
	}
	return 1;
}
void solve() {
	n = read();
	for (int i = 1; i <= n; i++) f[i][0] = read();
	vector<int> b(n + 1);
	for (int i = 1; i <= n; i++) b[i] = read();
	for (int j = 1; j <= lg[n]; j++) {
		for (int i = 1; i <= n - (1 << j) + 1; i++) {
			f[i][j] = max(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
		}
	}
	long long l = 0, r = 1e12, ans = 0;
	while (l < r) {
		long long mid = l + ((r - l) >> 1);
		if (check(mid, b)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans << endl;
}

int main() {
	lg[0] = -1;
	for (int i = 1; i <= 1e6; i++) lg[i] = lg[i >> 1] + 1;
	t = read();
	while (t--) solve();
	return 0;
}
