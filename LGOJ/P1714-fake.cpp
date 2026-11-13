#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 5;
long long n, m, cnt, p[N];
long long ans = LLONG_MIN, s[N], a[N];
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i) {
		scanf("%d", &a[i]);
		s[i] = s[i - 1] + a[i];
		ans = max(ans, a[i]);
		if (a[i] > 0) p[++cnt] = i; 
	}
	for (int i = 1; i + m - 1 <= n; ++i) {
		ans = max(ans, s[i + m - 1] - s[i - 1]);
	}
	shuffle(p + 1, p + cnt + 1, rnd);
	for (int i = 1; i <= cnt; ++i) {
		if ((double)clock() / CLOCKS_PER_SEC > 0.85) break;
		int l = p[i];
		int mxr = min(n, l + m - 1);
		for (int r = l; r <= mxr; ++r) {
			if (s[r] - s[l - 1] > ans) {
				ans = s[r] - s[l - 1];
			}
			if (s[r] - s[l - 1] <= 0) break; 
		}
	}
	printf("%lld\n", ans);
	return 0;
}
