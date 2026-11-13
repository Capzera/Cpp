#include <bits/stdc++.h>
using namespace std;
double w0, w, n;
int main() {
	cin >> w0 >> w >> n;
	double l = 0, r = 301, x;
	while (r - l >= 0.00001) {
		x = (l + r) / 2;
		double sum = 0, base = 1.0;
		for (int i = 1; i <= n; i++) {
			base *= 1.0 / (x + 1.0);
			sum += base;
		}
		sum *= w;
		if (sum == w0) break;
		if (sum > w0) l = x;
		else r = x;
	}
	cout << fixed << setprecision(1) << x * 100;
	return 0;
}
