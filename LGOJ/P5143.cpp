#include <bits/stdc++.h>
using namespace std;
struct point {
	double x, y, z;
	bool operator < (const struct point& rhs) {
		return z < rhs.z;
	}
} a[50010];
int n;
double ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].x >> a[i].y >> a[i].z;
	sort(a + 1, a + n + 1);
	for (int i = 2; i <= n; i++) {
		ans += sqrt(pow(a[i].x - a[i - 1].x, 2) + pow(a[i].y - a[i - 1].y, 2) + pow(a[i].z - a[i - 1].z, 2));
	}
	cout << fixed << setprecision(3) << ans;
	return 0;
}
