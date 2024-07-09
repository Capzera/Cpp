#include <bits/stdc++.h>
using namespace std;
int main() {
	char ch;
	long long a1, a2, b1, b2, k, x, y;
	cin >> ch >> a1 >> ch >> a2 >> ch >> b1 >> ch >> b2 >> k;
	x = a1 * (k + 1) + a2;
	y = b1 * (k + 1) + b2;
	cout << abs(x - y);
	return 0;
}
