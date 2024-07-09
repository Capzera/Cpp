#include <iostream>
using namespace std;

int main() {
	int a = 1, b = 1, c, m;
	cin >> m;
	for (int i = 0; i < m; i++) {
		c = a + b;
		a = b;
		b = c;
	}
	cout << c;
	return 0;
}
