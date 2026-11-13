#include <bits/stdc++.h>
using namespace std;
long long a, b, c, d, e, f;
int main() {
	cin >> a >> b >> c >> d >> e >> f;
	a = a * d + b * e;
	c = (a + c - 1) / c;
	cout << (c + f - 1) / f;
	return 0;
}
