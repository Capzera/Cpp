#include <bits/stdc++.h>
using namespace std;
long long N = 12, x, sum;
double f(long long x) {
	if (x <= 310) return 3.0 * x;
	if (x <= 520) return 3.3 * x - 93;
	return 4.2 * x - 561;
}
double ans;
int main() {
	while (N--) {
		cin >> x;
		if (sum + x < 310) ans = 3.0 * x;
		cout << fixed << setprecision(1) << f(sum + x) - f(sum) << endl;
		sum += x;
	}
	return 0;
}
