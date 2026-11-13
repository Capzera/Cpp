#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
typedef long long ll;
const int dx[] = {-2, -1, 0, 1, 2};
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
int n, m, d;
ll a[N], sum[N], id[N];
int main() {
	n = read(), m = read(), d = read();
	for (int i = 1; i < n; i++) {
		int b = read();
		if (d == 1) {
			a[i] = max(0, (b - 2) / 2);
		} else {
			if (b < 2 * d + 1) a[i] = 0;
			else a[i] = (b - (2 * d + 1)) / (2 * d) + 1;
		}
	}
	int block_cnt = 0;
	int last = 0;
	for (int i = 1; i < n; i++) {
		if (a[i] == 0) continue;
		if (last != 0 && i - last <= 2) {
			id[i] = id[last];
			sum[id[i]] += a[i];
		} else {
			block_cnt++;
			id[i] = block_cnt;
			sum[block_cnt] = a[i];
		}
		last = i;
	}
	ll ans = 0;
	if (a[m] > 0) ans = sum[id[m]];
	for (int x = -2; x <= 2; x++) {
		int pos = m + x;
		if (pos < 1 || pos >= n) continue;
		if (a[pos] > 0) ans = max(ans, sum[id[pos]]);
	}
	cout << ans << '\n';
	return 0;
}
