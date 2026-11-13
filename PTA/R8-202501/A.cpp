#include <bits/stdc++.h>
using namespace std;
struct node {
	int t, v;
	bool operator < (const struct node& rhs) const {
		return v * rhs.t > rhs.v * t;
	}
} a[1001];
int n, m, v;
double ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].t >> a[i].v;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n && m; i++) {
		int cost = min(m, a[i].t);
		ans += 1.0 * cost / a[i].t * a[i].v;
		m -= cost;
	}
	printf("%.2lf", ans);
	return 0;
}
