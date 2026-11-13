#include <bits/stdc++.h>
#define yn(x) !x ? puts("Yes") : puts("No");
using namespace std;
const int N = 1e6 + 10;
int T, n, q, op, p;
char c, s[N], t[N];
void solve() {
	scanf("%d%d%s%s", &n, &q, s + 1, t + 1);
	int cnt = 0;
	for (int i = 1; i <= n; i++) {
		cnt += s[i] != t[i];
	}
	yn(cnt);
	while (q--) {
		scanf("%d%d %c", &op, &p, &c);
		cnt -= s[p] != t[p];
		op == 0 ? s[p] = c : t[p] = c;
		cnt += s[p] != t[p];
		yn(cnt);
	}
}
int main() {
	scanf("%d", &T);
	while (T--) solve();
	return 0;
}
