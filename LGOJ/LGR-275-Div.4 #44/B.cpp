#include <bits/stdc++.h>
using namespace std;
int a, b, x, y, m;
int main() {
	cin >> a >> b >> x >> y >> m;
	if (x > y) swap(x, y), swap(a, b);
	cout << min(m / x, a) + min(max(m - a * x, 0) / y, b);
	return 0;
}
