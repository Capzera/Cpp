#include <bits/stdc++.h>
using namespace std;
double n, ans, Euler = 0.5772156649015328;
int main() {
	cin >> n;
	if (n <= 2e3) {
		for (int i = 1; i <= n; i++) ans += 1.0 / i;
	} else ans = Euler + log(n) + 0.5 / n;
	printf("%.8lf", ans);
	return 0;
}
