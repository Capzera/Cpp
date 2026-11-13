#include <bits/stdc++.h>
#if defined (_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#endif
using namespace std;
typedef long long ll;
const int N = 1e5 + 10;
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
inline void write(ll x) {
	if (x < 0) putchar_unlocked('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar_unlocked(x % 10 + '0');
}
ll n, m, k, x, ans, sum, c[N], a[N];
int main() {
	n = read(), m = read(), k = read();
	for (int i = 1; i < n; i++) c[i + 1] = c[i] + (x = read());
	for (int i = 1; i <= n; i++) a[i] = read();
	priority_queue<ll, vector<ll>, greater<ll> > pq;
	for (int i = 1; i <= n; i++) {
		if (pq.size() < k) {
			pq.push(a[i]);
			sum += a[i];
			continue;
		}
		if (a[i] > pq.top()) {
			sum -= pq.top() - a[i]; pq.pop();
			pq.push(a[i]);
		}
		ans = max(ans, max(0ll, min(m - c[i], sum)));
	}
	write(ans);
	return 0;
}
