#include <bits/stdc++.h>
#if defined (_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#endif
using namespace std;
typedef long long ll;
const int N = 1e6 + 5;
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
struct Node {
	ll f, a;
	void read() {
		f = ::read(), a = ::read();
	}
	bool operator<(const Node& rhs) const {
		return f < rhs.f;
	}
} a[N];
ll n, x, y, w, sum, ans;
int main() {
	n = read(), x = read(), y = read(), w = read();
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		ll cnt = (sum + a[i].a - 1) / w;
		if (!sum) cnt++;
		else cnt -= (sum - 1) / w;
		ans += cnt * 2LL * (y - a[i].f);
		sum += a[i].a;
	}
	// y - min - |x - min|;
	ans -= y - a[1].f - abs(x - a[1].f);
	write(ans); 
	return 0;
}
