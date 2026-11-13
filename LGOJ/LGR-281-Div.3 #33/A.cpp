#include <bits/stdc++.h>
using namespace std;
inline long long read() {
	long long ans = 0, f = 1;
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
inline void write(long long x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar(x % 10 + '0');
}
long long t, n, m, k;

void solve() {
	n = read(), m = read(), k = read();
	if (!m) {
		write(n);
		putchar('\n');
		return;
	}
	n %= k; m--;
	if (n + m >= k) write(0);
	else write(n);
	putchar('\n');
}

int main() {
	t = read();
	while (t--) solve();
	return 0;
}
