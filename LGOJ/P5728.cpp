#include <bits/stdc++.h>
using namespace std;
int n, m, a[20];
double ans;
int main() {
	cin >> n >> m;
	while (n--) {
		for (int i = 0; i < m; i++) cin >> a[i];
		int cnt = 0;
		sort(a, a + m);
		for (int i = 1; i < m - 1; i++) cnt += a[i];
		ans = max(ans, 1.0 * cnt / (m - 2));
	}
	printf("%.2lf", ans);
	return 0;
}
