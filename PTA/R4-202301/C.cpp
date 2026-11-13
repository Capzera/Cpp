#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct node {
	long long w, v;
	double pri;
	void read() {
		cin >> w >> v;
	}
	bool operator < (const struct node& rhs) const {
		return v * rhs.w > w * rhs.v;
	}
} a[N];
long long n, m;
double ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (!a[i].w) continue;
		int cost = min(m, a[i].w);
		ans += cost * a[i].v / a[i].w;
		m -= cost;
	}
	printf("%.2lf", ans);
	return 0;
}
