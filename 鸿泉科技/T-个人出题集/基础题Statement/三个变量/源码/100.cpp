#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, b, c, d;
	cin >> a >> b >> c;
	d = c;
	c = b;
	b = a;
	a = d;
	cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
	return 0;
}
