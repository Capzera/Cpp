#include <bits/stdc++.h>
using namespace std;
long long a, b, x, y;
int main() {
	cin >> a >> b >> x >> y;
	if (a > b) swap(a, b);
	if (x > y) swap(x, y);
	if (y < a || x > b) {
		cout << b - a;
		return 0;
	}
	cout << min(abs(x - a) + abs(y - b), b - a);
	return 0;
}
