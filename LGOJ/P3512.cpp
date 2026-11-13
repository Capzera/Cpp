#include <bits/stdc++.h>
using namespace std;
const int N = 3e6 + 10;
int n, k, ans, a[N];
int h1 = 1, t1, h2 = 1, t2, qmx[N], qmn[N];
int main() {
	scanf("%d%d", &k, &n);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for (int i = 1, lt = 1; i <= n; i++) {
		while (h1 <= t1 && a[qmn[t1]] >= a[i]) t1--;
		qmn[++t1] = i;
		while (h2 <= t2 && a[qmx[t2]] <= a[i]) t2--;
		qmx[++t2] = i;
		while (h1 <= t1 && h2 <= t2 && a[qmx[h2]] - a[qmn[h1]] > k) {
			if (qmx[h2] < qmn[h1]) {
				lt = qmx[h2++] + 1;
			} else {
				lt = qmn[h1++] + 1;
			}
		}
		ans = max(ans, i - lt + 1);
	}
	printf("%d\n", ans);
	return 0;
}
