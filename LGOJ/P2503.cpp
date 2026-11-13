#include <bits/stdc++.h>
using namespace std;
const int N = 25;
int n, m, a[N], sum[N];
double avg, ans = 1e18;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		avg += a[i];
	}
	avg /= m;
	while ((double)clock() / CLOCKS_PER_SEC < 0.9) {
		shuffle(a + 1, a + n + 1, rnd);
		for (int i = 1; i <= m; i++) sum[i] = 0;
		for (int i = 1; i <= n; i++) {
			int mn = 1;
			for (int j = 2; j <= m; j++) {
				if (sum[j] < sum[mn]) mn = j;
			}
			sum[mn] += a[i];
		}
		double cur = 0;
		for (int i = 1; i <= m; i++) {
			cur += (sum[i] - avg) * (sum[i] - avg);
		}
		ans = min(ans, sqrt(cur / m));
	}
	printf("%.2lf\n", ans);
	return 0;
}
