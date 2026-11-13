#include <bits/stdc++.h>
using namespace std;
long long a, b, c, d;
char ch;
int main() {
	cin >> a >> ch >> b >> c >> ch >> d;
	a *= c, b *= d;
	long long g = __gcd(a, b);
	cout << b / g << " " << a / g;
	return 0;
}
