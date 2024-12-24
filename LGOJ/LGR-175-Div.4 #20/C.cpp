#include <bits/stdc++.h>
using namespace std;
long long n, m, a, b;
double c1, c2;
int main() {
	cin >> n >> m;
	while (n--) {
		cin >> a >> b;
		if (a == 1) c1 += b;
		c2 += b;
	}
	cout << fixed << setprecision(3) << c1 / c2 * m;
	return 0;
}
