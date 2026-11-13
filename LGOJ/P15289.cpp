#include <bits/stdc++.h>
#if defined(_WIN32) || defined(_WIN64)
	#define getchar_unlocked _getchar_nolock
#endif
#define ll long long
using namespace std;
const int N = 1e6 + 10;
inline ll read() {
	ll ans = 0, f = 1;
	char ch = getchar_unlocked();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar_unlocked();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar_unlocked();
	}
	return ans * f;
}

ll n, m, d, ans;
ll a[N], sum[N], id[N];

int main() {
	n = read(), m = read(), d = read();
	for (int i = 1; i < n; i++) {
		ll x = read();
		if (x >= 2ll * d + 2) a[i] = (x - 2) / (2 * d);
	}
	for (int i = 1, last = 0, cnt = 0; i < n; i++) {
		if (a[i] == 0) continue;
		if (last && i - last <= 2) {
			id[i] = id[last];
			sum[id[i]] += a[i];
		} else {
			id[i] = ++cnt;
			sum[cnt] = a[i];
		}
		last = i;
	}
	for (int x = -2; x <= 2; x++) {
		ll mx = m + x;
		if (mx < 1 || mx >= n) continue;
		if (a[mx] > 0) ans = max(ans, sum[id[mx]]);
	}
	printf("%lld\n", ans);
	return 0;
}
