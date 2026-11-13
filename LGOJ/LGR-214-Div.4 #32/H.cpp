#include <bits/stdc++.h>
using namespace std;
long long cal(long long ax, long long ay, long long bx, long long by) {
	return max(0ll, min(ay, by) - max(ax, bx) + 1);
}
int main() {
	long long x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	cin >> x1 >> y1 >> z1;
	cin >> x2 >> y2 >> z2;
	cin >> x3 >> y3 >> z3;
	cin >> x4 >> y4 >> z4;
	long long x = cal(x1, x2, x3, x4);
	long long y = cal(y1, y2, y3, y4);
	long long z = cal(z1, z2, z3, z4);
	cout << 1ll * x * y * z;
	return 0;
}
