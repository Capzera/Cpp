#include <bits/stdc++.h>
using namespace std;
double k, f, c;
int main() {
	cin >> k;
	if (k > 300) return puts("Temperature is too high!"), 0;
	c = k - 273.15;
	f = c * 1.8 + 32;
	printf("%.2lf %.2lf", c, f);
	return 0;
}
