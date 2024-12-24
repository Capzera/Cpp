#include <bits/stdc++.h>
using namespace std;
double a, b, c, d, e, x, y, id = 1, ans;
int main() {
	cin >> a >> b >> c >> d >> e >> x >> y;
	id = 1, ans = a * x + b * y;
	if (c * y > ans) {
		ans = c * y;
		id = 2;
	}
	if (d * y + e > ans) {
		ans = d * y + e;
		id = 3;
	}
	cout << id << " " << fixed << setprecision(2) << ans;
	return 0;
}
