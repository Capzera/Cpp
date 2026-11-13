#include <bits/stdc++.h>
using namespace std;
long long n, x, y, z, c1, c2, c3;
int main() {
	cin >> n;
	while (n--) {
		cin >> x >> y >> z;
		c1 += 8 - x, c2 += 12 - y, c3 += 1 - z;
	}
	cout << min({c1 / 8, c2 / 12, c3});
	return 0;
}
