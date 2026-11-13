#include <bits/stdc++.h>
using namespace std;
const int N = 1005, mod = 1000002013;
int t, n, m, st, ed, ti, top;
struct node {
	long long pos, car, times;
	bool operator < (const struct node& rhs) const {
		if (pos != rhs.pos) return pos < rhs.pos;
		return car > rhs.car;
	}
} stk[N];
long long calc(long long d) {
	return ((n * 2 - d + 1) * d >> 1) % mod;
}
long long solve() {
	scanf("%d%d", &n, &m);
	vector<node> a;
	long long sum = 0, ans = 0;
	top = 0;
	for (int i = 1; i <= m; i++) {
		scanf("%d%d%d", &st, &ed, &ti);
		a.push_back({st, 1, ti});
		a.push_back({ed, 0, ti});
		sum = (sum + calc(ed - st) * ti) % mod;
	}
	sort(a.begin(), a.end());
	for (auto& p : a) {
		if (p.car == 1) {
			stk[++top] = p;
			continue;
		}
		while (top && p.times) {
			long long cnt = min(p.times, stk[top].times);
			ans = (ans + calc(p.pos - stk[top].pos) * cnt) % mod;
			stk[top].times -= cnt;
			p.times -= cnt;
			if (!stk[top].times) top--;
		}
	}
	return (sum - ans + mod) % mod;
}
int main() {
	scanf("%d", &t);
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}
