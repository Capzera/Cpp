#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, m, c, f, a[N];
int h1 = 1, t1, h2 = 1, t2, qmx[N], qmn[N];
int main() {
	scanf("%d%d%d", &n, &m, &c);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for (int i = 1; i <= n; i++) {
		while (h1 <= t1 && a[qmn[t1]] >= a[i]) t1--;
		qmn[++t1] = i;
		while (h2 <= t2 && a[qmx[t2]] <= a[i]) t2--;
		qmx[++t2] = i;
		while (h1 <= t1 && qmn[h1] <= i - m) h1++;
		while (h2 <= t2 && qmx[h2] <= i - m) h2++;
		if (i < m) continue;
		if (a[qmx[h2]] - a[qmn[h1]] <= c) {
			printf("%d\n", i - m + 1);
			f = 1;
		}
	}
	if (!f) puts("NONE");
	return 0;
}
