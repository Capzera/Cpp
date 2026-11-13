#include <bits/stdc++.h>
using namespace std;
const int N = 2000005;
long long n, k, p, a, b, ans, mn;
long long f[N], tot[N], mp[N];
int main() {
	scanf("%lld%lld%lld", &n, &k, &p);
	for (register int i = 1; i <= n; ++i) {
		scanf("%lld%lld", &a, &b);
		if (b <= p) mn = i;
		if (mn >= f[a]) tot[a] = mp[a];
		ans += tot[a];
		f[a] = i;
		mp[a]++;
	}
	printf("%lld", ans);
	return 0;
}
