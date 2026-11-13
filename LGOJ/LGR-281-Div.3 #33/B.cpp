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
long long solve() {
	n = read(), m = read(), k = read();
	if (!k) return n;
	if (n < m) return 0;
	for (int i = 1; i <= min(k, 200ll); i++) {
		n = n & max(n - m, 0ll);
	}
	return n;
}
int main() {
	t = read();
	while (t--) cout << solve() << '\n';
	return 0;
}
