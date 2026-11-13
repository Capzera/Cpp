#include <bits/stdc++.h>
using namespace std;
double x, n;
double f(double n) {
	if (n == 1) return sqrt(x + 1);
	return sqrt(n + f(n - 1));
}
int main() {
	cin >> x >> n;
	printf("%.2lf", f(n));
	return 0;
}
