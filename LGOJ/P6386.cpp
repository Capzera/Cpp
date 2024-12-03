#include <iostream>
using namespace std;
int a, b, c, d, x, y, z;
string mp[] = {"none", "one", "both"};
void f(int x) {
	cout << mp[(x % (a + b) < a) + (x % (c + d) < c)] << endl;
}
int main() {
	cin >> a >> b >> c >> d >> x >> y >> z;
	f(x - 1), f(y - 1), f(z - 1);
	return 0;
}

