#include <bits/stdc++.h>
using namespace std;
int main() {
	freopen("truck.in", "r", stdin);
	freopen("truck.out", "w", stdout);
	long long a, b, x, k, ans = 0, cnt = 0;
	cin >> a >> b >> x >> k;
	while (a) {
		ans += a * x;
		cnt += a;
		a = min(cnt / k, b);
		cnt %= k;
		b -= a;
	}
	cout << ans;
	return 0;
}
