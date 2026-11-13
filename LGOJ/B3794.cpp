#include <bits/stdc++.h>
using namespace std;
long long a, b, c;
double pi = 3.1415926535;
int main() {
	cin >> a >> b >> c;
	double ans = pi * c * c / 4 + a * c + b * (a + c);
	printf("%.3lf", ans / 10000);
	return 0;
}
