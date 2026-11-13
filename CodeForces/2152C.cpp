#include <bits/stdc++.h>
using namespace std;
const int N = 250005;
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
int t, n, q, l, r, a[N], sum[N], diff[N];
int get(int l, int r) {
	int len = r - l + 1;
	if ((sum[r] - sum[l - 1]) % 3 != 0 || len % 3) return -1;
	return len / 3 + (diff[r] - diff[l] == len - 1);
}
void solve() {
	n = read(), q = read();
	memset(sum, 0, sizeof(sum));
	memset(diff, 0, sizeof(diff));
	for (int i = 1; i <= n; i++) {
		a[i] = read();
		sum[i] = sum[i - 1] + (a[i] ? 1 : -1);
		diff[i] = diff[i - 1] + (a[i] != a[i - 1]);
	}
	while (q--) {
		l = read(), r = read();
		printf("%d\n", get(l, r));
	}
}
int main() {
	t = read();
	while (t--) solve();
	return 0;
}

