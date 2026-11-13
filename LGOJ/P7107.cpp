#include <bits/stdc++.h>
using namespace std;
long long n, m, k, p, t, L, R;
int main() {
	cin >> n >> m >> k >> p;
	if (!p) return puts("NO"), 0;
	L = (n + k - p + n - 1) / n;
	R = min(m, k / p);
	if (L > R) return puts("NO"), 0;
	t = L;
	puts("YES");
	for (int i = 1; i <= p; i++) {
		cout << t << " " << m - t << '\n';
	}
	if (p == n) return 0;
	long long rest = k - p * t, each = rest / (n - p), rem = rest % (n - p);
	for (int i = p + 1; i <= n; i++) {
		int cur = each + (i - p <= rem);
		cout << cur << " " << m - cur << '\n';
	}
	return 0;
}
