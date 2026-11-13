#include <bits/stdc++.h>
using namespace std;
long long n, ans, a[1000001], l[1000001], r[1000002];
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-')f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
int main() {
	n = read();
	l[0] = r[n + 1] = LLONG_MAX >> 1;
	for (int i = 1; i <= n; i++) {
		a[i] = read();
		a[i] += a[i - 1];
		l[i] = min(l[i - 1], a[i]);
	}
	for (int i = n; i > 0; i--) r[i] = min(r[i + 1], a[i]);
	for (int i = 1; i <= n; i++) {
		ans += r[i] - a[i - 1] >= 0 && l[i - 1] + a[n] - a[i - 1] >= 0;
	}
	cout << ans;
	return 0;
}

