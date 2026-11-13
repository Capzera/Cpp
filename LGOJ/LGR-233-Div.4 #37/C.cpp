#include <bits/stdc++.h>
using namespace std;
long long n, a, b, x, y;
int main() {
	cin >> n >> x >> y;
	while (n--) {
		cin >> a >> b;
		if (x < a) break;
		x += b - a;
		if (x >= y) break;
	}
	cout << x << endl;
	return 0;
}
