#include <bits/stdc++.h>
using namespace std;
long long a, b, c, d, e, f, g, p, l1, r1, l2, r2, ans;
int main() {
	cin >> a >> b >> c >> d >> e >> f >> g >> p >> l1 >> r1 >> l2 >> r2;
	for (int x = l1; x <= r1; x++)
		for (int y = l2; y <= r2; y++)
			ans = max(ans, (a * x * x * x + b * y * y * y + c * y * x * x + d * x * y * y + e * x * y + f * x + g * y) % p);
	cout << ans;
	return 0;
}
