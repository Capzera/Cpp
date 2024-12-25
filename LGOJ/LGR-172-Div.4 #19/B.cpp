#include <bits/stdc++.h>
using namespace std;
long long a, b, c, mn = INT_MAX, x, y;
void fun(int a, int b) {
	if (abs(a - b) < mn) {
		mn = abs(a - b);
		x = max(a, b), y = min(a, b);
	}
}
int main() {
	cin >> a >> b >> c;
	fun(a, b + c);
	fun(b, a + c);
	fun(c, a + b);
	cout << x << " " << y << endl;
	return 0;
}
